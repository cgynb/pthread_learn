#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

const int thread_count = 5;
int counter = 0;
pthread_mutex_t mutex;
pthread_cond_t cond_var;

void* Hello(void* arg){
    if((long)arg == 2){
        sleep(2);
    }
    pthread_mutex_lock(&mutex);
    counter++;
    if(counter == thread_count){
        counter = 0;
        pthread_cond_broadcast(&cond_var);
    }else{
        while(pthread_cond_wait(&cond_var, &mutex) != 0){}
    }
    pthread_mutex_unlock(&mutex);
    printf("tid: %ld counter = %d\n", (long)arg, counter);
}

int main(){
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_var, NULL);

    pthread_t* thread_handles = (pthread_t*)malloc(thread_count * sizeof(pthread_t));
    for(int tid=0; tid<thread_count; tid++){
        pthread_create(&thread_handles[tid], NULL, Hello, tid);
    }
    for(int tid=0; tid<thread_count; tid++){
        pthread_join(thread_handles[tid], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_var);
}