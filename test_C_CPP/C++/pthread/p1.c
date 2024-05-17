#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

void *task(void *arg)
{
    //线程处理函数
    printf("this is a task tid = %ld\n",pthread_self());
}

int main()
{
    pthread_t tid;

    //创建线程
    if(pthread_create(&tid,NULL,task,NULL)!=0){
            printf("pthread_create is error\n");
            return 0;
    }
    sleep(1);
    printf("这是主线程 = %ld\n",tid);
    while(1);
    return 1;
}