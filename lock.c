#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

const int n = 10000000;
int sum = 0;
int thread_count;
pthread_mutex_t mutex;


void* cul_sum(void* tid){
    int start = (n / thread_count) * (long)tid;
    int end = start + (n / thread_count);
    printf("%d: %d %d\n", tid, start, end);
    int my_sum = 0;
    for(int i=start; i<end; i++){
        my_sum += i;
    }
    pthread_mutex_lock(&mutex);
    sum += my_sum;
    pthread_mutex_unlock(&mutex);
}

int main(int argc, char* argv[]){
    pthread_mutex_init(&mutex, NULL);
    thread_count = strtol(argv[1], NULL, 10);
    pthread_t* thread_handles = (pthread_t*)malloc(thread_count * sizeof(pthread_t));
    for(int tid=0; tid<thread_count; tid++){
        pthread_create(&thread_handles[tid], NULL, cul_sum, (void*)tid);
    }
    for(int tid=0; tid<thread_count; tid++){
        pthread_join(thread_handles[tid], NULL);
    }
    pthread_mutex_destroy(&mutex);
    printf("sum = %d", sum);
    return 0;
}