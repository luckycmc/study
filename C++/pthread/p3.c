#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

int num = 10;
//线程指向的函数体
void *task(void *arg)
{
    //线程处理函数
    printf("this is a task tid = %ld\n",pthread_self());
    while (1)
    {
        printf("I am pthread.....\n");
        exit(1);  //在线程中使用会导致整个进程退出
        //sleep(1);
        //当前线程退出 只退出当前线程
        //pthread_exit(NULL);
       
    }
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
    while(1){
        printf("i am process,num = %d\n",num++);
        sleep(1);
    }
    return 1;
}