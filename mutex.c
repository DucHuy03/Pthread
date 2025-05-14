#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
int counter = 0;
pthread_mutex_t lock;
void *increment(){
    for(int i = 0; i < 1000000; i++){
        pthread_mutex_lock(&lock);
        counter++;
        pthread_mutex_unlock(&lock);
    }
}

int main(){
    pthread_t th[10];
    pthread_mutex_init(&lock, NULL);
    for(int i = 0; i < 10; i++){
        if(pthread_create(&th[i], NULL, &increment, NULL) != 0){
            perror("Failed to create thread");
        }        
    }
    for(int i = 0; i < 10; i++){
        pthread_join(th[i], NULL);
    }
    printf("Final counter value: %d\n", counter);
    pthread_mutex_destroy(&lock);
    return 0;   
}