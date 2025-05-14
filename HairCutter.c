#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#define NUM_CHAIRS 5
#define NUM_CUSTOMERS 20
#define NUM_BARBERS 2
int waiting = 0;
pthread_mutex_t lock;
pthread_cond_t barber_cond;
pthread_cond_t customer_cond;
void *barber(void *arg){ //Thợ sẽ ngủ nếu kh có khách và đợi biến điều kiện barber_cond
    while(1){
        pthread_mutex_lock(&lock); // khóa tài nguyên chung
        while(waiting == 0){
            printf("Barber is sleeping...\n");
            pthread_cond_wait(&barber_cond, &lock);
        }
        waiting--; // giảm số lượng khách đang chờ và đánh thức khách rằng có chỗ trống
        printf("Barber is cutting hair. Customers waiting: %d\n", waiting);
        pthread_cond_signal(&customer_cond);
        pthread_mutex_unlock(&lock);
        sleep(1);
    }
}
void *customer(void *arg){
    pthread_mutex_lock(&lock);
    if(waiting < NUM_CHAIRS){ // nếu có chỗ trống thì khách sẽ ngồi chờ
        waiting++; //  tăng số lượng khách đang chờ
        printf("Customer %d is waiting. Customers waiting: %d\n", *(int *)arg, waiting);
        pthread_cond_signal(&barber_cond);// Thông báo rằng có khách đang chờ
        pthread_cond_wait(&customer_cond, &lock); // Chờ đến lượt
        printf("Customer %d is getting a haircut.\n", *(int *)arg);
    } else {
        printf("Customer %d leaves without getting a haircut. No chairs available.\n", *(int *)arg);
    }
    pthread_mutex_unlock(&lock);
}
void *customer_thread(void *arg){
    customer(arg);
    return NULL;
}
int main(){
    pthread_t barbers[NUM_BARBERS];
    pthread_t customers[NUM_CUSTOMERS];
    int customer_ids[NUM_CUSTOMERS];

    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&barber_cond, NULL);
    pthread_cond_init(&customer_cond, NULL);

    for(int i = 0; i < NUM_BARBERS; i++){
        pthread_create(&barbers[i], NULL, barber, NULL);
    }

    for(int i = 0; i < NUM_CUSTOMERS; i++){
        customer_ids[i] = i + 1;
        pthread_create(&customers[i], NULL, customer_thread, &customer_ids[i]);
        sleep(1); // thời gian giữa các khách hàng đến
    }

    for(int i = 0; i < NUM_CUSTOMERS; i++){
        pthread_join(customers[i], NULL);// Chờ tất cả khách hàng hoàn thành
    }

    for(int i = 0; i < NUM_BARBERS; i++){
        pthread_cancel(barbers[i]);// Hủy tiến trình thợ cắt tóc
    }

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&barber_cond);
    pthread_cond_destroy(&customer_cond);

    return 0;
}
