#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

const int thread_count = 5;
int counter = 0;
sem_t count_sem, barrier_sem;

void* Hello(void* arg){
    printf("tid = %d\n", (long)arg);

    // 进行阻塞, 将count_sem的信号量减为0, 保护counter, 类似于上锁操作吧
    sem_wait(&count_sem);

    // 挂起线程4
    if((long)arg == 4){
        sleep(2);
    }
    if(counter == thread_count-1){
        counter = -1;
        // 对应起上面的sem_wait(&count_sem);
        // 类似于开锁的操作
        sem_post(&count_sem);
        for(int i=0; i<thread_count; i++){
            // 将信号量加到thread_count
            sem_post(&barrier_sem);
        }
    }else{
        counter++;
        // 对应起上面的sem_wait(&count_sem);
        // 类似于开锁的操作
        sem_post(&count_sem);
        // 在运行最后一个线程前, 这里会一直阻塞着

        /*
            在最后一个线程做完thread_count次sem_post(&barrier_sem);之后
            barrier_sem的信号量就变成thread_count
            此时barrier_sem的信号量就可以开始减-1
        */
        sem_wait(&barrier_sem);
    }
    printf("thread %d, counter=%d\n", (long)arg, counter);
}

int main(){
    sem_init(&count_sem, 0, 1);  // 将count_sem的值初始化为1, 作用是保护counter
    sem_init(&barrier_sem, 0, 0);  // 将barrier_sem的值初始化为0, 路障
    pthread_t* thread_handles;
    thread_handles = malloc(thread_count * sizeof(pthread_t));
    for(int tid=0; tid<thread_count; tid++){
        pthread_create(&thread_handles[tid], NULL, Hello, (void*)tid);
    }
    for(int tid=0; tid<thread_count; tid++){
        pthread_join(thread_handles[tid], NULL);
    }
    sem_destroy(&count_sem);
    sem_destroy(&barrier_sem);
    return 0;
}