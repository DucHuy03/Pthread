
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#define BUFFER_SIZE 10
int buffer[BUFFER_SIZE];
int in = 0;
int out = 0;
pthread_mutex_t lock;   // Khóa mutex bảo vệ vùng sử dụng chung
pthread_cond_t not_empty; // Biến điều kiện, nếu không rỗng thì wake up consumer
pthread_cond_t not_full; // Biến điều kiện, nếu không đầy thì wake up producer
void *producer(void *arg){
    for(int i = 0; i < 20; i++){
        pthread_mutex_lock(&lock); // Khóa trước khi truy cập buffer
        while((in + 1) % BUFFER_SIZE == out){ // Nếu đầy thì chờ biến điều kiện not_full và unlock mutex
            pthread_cond_wait(&not_full, &lock);
        }
        buffer[in] = i; // Thêm phần tử vào buffer
        printf("Produced: %d\n", buffer[in]);
        in = (in + 1) % BUFFER_SIZE; // tăng số phần tử trong buffer
        pthread_cond_signal(&not_empty); // đánh thức consumer vì buffer đã có phần tử
        pthread_mutex_unlock(&lock);// mở khóa
    }
}
void *consumer(void *arg){
    for(int i = 0; i < 20; i++){
        pthread_mutex_lock(&lock); // Khóa trước khi truy cập buffer
        while(in == out){ // Nếu buffer rỗng thì chờ biến điều kiện not_empty và unlock mutex
            pthread_cond_wait(&not_empty, &lock);
        }
        printf("Consumed: %d\n", buffer[out]); // lấy ra 1 phần tử trong buffer
        out = (out + 1) % BUFFER_SIZE; // giảm số phần tử trong buffer, buffer vòng nên tăng out [in ..... out]
        pthread_cond_signal(&not_full); // đánh thức producer vì buffer đã có chỗ trống
        pthread_mutex_unlock(&lock); // mở khóa
    }
}
int main(){
    pthread_t prod, cons; // tạo 2 thread producer và consumer
    pthread_mutex_init(&lock, NULL); // khởi tạo khóa
    pthread_cond_init(&not_empty, NULL); // khởi tạo biến điều kiện
    pthread_cond_init(&not_full, NULL); // khởi tạo biến điều kiện
    
    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);
    
    pthread_join(prod, NULL);
    pthread_join(cons, NULL);
    
    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&not_empty);
    pthread_cond_destroy(&not_full);
    
    return 0;
}
