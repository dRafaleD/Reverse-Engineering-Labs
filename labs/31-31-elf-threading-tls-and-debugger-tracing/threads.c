#include <pthread.h>
#include <stdio.h>
static _Thread_local int local_counter = 0;
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
static int shared_total = 0;
static void *worker(void *arg) {
    int id = *(int *)arg;
    for (int i=0; i<3; ++i) {
        ++local_counter;
        pthread_mutex_lock(&lock);
        ++shared_total;
        pthread_mutex_unlock(&lock);
    }
    printf("thread=%d local=%d\n", id, local_counter);
    return NULL;
}
int main(void) {
    pthread_t threads[2]; int ids[2]={1,2};
    for(int i=0;i<2;i++) if(pthread_create(&threads[i],NULL,worker,&ids[i])) return 1;
    for(int i=0;i<2;i++) pthread_join(threads[i],NULL);
    printf("shared_total=%d\n",shared_total);
    return 0;
}
