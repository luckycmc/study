#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * 借助互斥锁管理共享数据的实现
 * 
 * */

void *tfn(void *arg)
{
    srand(time(NULL));
    while(1)
    {
         printf("hello ");
         sleep(rand() % 3); /*模拟长时间操作共享资源，导致 cpu 易主，产生与时间有关的错误*/
         printf("world\n");
         sleep(rand() % 3);
    }
    
    return NULL;
}

//主函数
int  main()
{
    pthread_t tid;
    srand(time(NULL));

    pthread_create(&tid, NULL,tfn,NULL);

    while(1)
    {
        printf("HELLO ");
        sleep(rand() % 3);
        printf("WORLD\n");
        sleep(rand() % 3);
    }

    pthread_join(tid, NULL);
    return 0;
}