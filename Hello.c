#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
void *print(){
    printf("Hello from thread %ld\n", pthread_self());
}
int main(){
    pthread_t th[10];
    for(int i = 0; i< 10; i++){
        if(pthread_create(&th[i], NULL, &print, NULL) != 0){
            perror("Failed to create thread");
        }        
    }

    for(int i = 0; i < 10; i++){
        pthread_join(th[i], NULL);
    }
    return 0;
}