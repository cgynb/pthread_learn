#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

const int thread_count = 5;
int counter = 0;
pthread_mutex_t mutex;

void* Hello(void* arg){
    printf("thread %ld start\n", (long)arg);

    // 给计数器上锁
    pthread_mutex_lock(&mutex);
    counter++;
    pthread_mutex_unlock(&mutex);

    // 挂起线程3
    if((long)arg == 3){
        printf("wating for thread 3");
        sleep(2);
    }

    // 忙等待
    while(counter < thread_count){}
    
    // 这段代码是同步的
    printf("thread %ld end, counter = %d\n", (long)arg, counter);
    return NULL;
}

int main(){
    pthread_t* thread_handles;
    pthread_mutex_init(&mutex, NULL);
    thread_handles = malloc(thread_count * sizeof(pthread_t));
    for(int tid=0; tid<thread_count; tid++){
        pthread_create(&thread_handles[tid], NULL, Hello, (void*)tid);
    }
    for(int tid=0; tid<thread_count; tid++){
        pthread_join(thread_handles[tid], NULL);
    }
    pthread_mutex_destroy(&mutex);
    return 0;
}