#include <stdio.h>

#include <pthread.h>
#include <unistd.h>

#include <sys/mman.h>

#define THREAD_SIZE     10  //定义线程的数量

int count = 0;
pthread_mutex_t mutex;  //定义一个排它锁
void *func(void *arg) {

    int *pcount = (int *)arg;

    int i = 0;
    while (i++ < 100000) {
         (*pcount) ++;
         usleep(1);
    }
}
int main()
{
     pthread_t threadid[THREAD_SIZE] = {0};

    pthread_mutex_init(&mutex, NULL);
    pthread_spin_init(&spinlock, PTHREAD_PROCESS_SHARED);
    pthread_rwlock_init(&rwlock, NULL);

    int i = 0;
    int count = 0;
    for (i = 0;i < THREAD_SIZE;i ++) {
        int ret = pthread_create(&threadid[i], NULL, func, &count);
        if (ret) {
            break;
        }
    }

    for (i = 0;i < 100;i ++) {
        pthread_rwlock_rdlock(&rwlock);
        printf("count --> %d\n", count);
        pthread_rwlock_unlock(&rwlock);

        sleep(1);
    }
}