#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

//错误信息打印
void sys_err(const char *str)
{
    perror(str);
    exit(1);
}

//进程执行的函数

void *tfn(void *arg)
{   
    int i = *((int *)arg);
    printf("--I'm %dth thread: pid = %d, tid = %lu\n",i+1, getpid(), pthread_self());
    return NULL;
}
//主函数执行
int main()
{
     pthread_t tid;
     
     int i; 
     printf("main: pid = %d, tid = %lu\n", getpid(), pthread_self());

     //创建线程的回调函数
     //循环创建多个子线程
     for ( i = 0; i < 5; i++)
     {
         int ret = pthread_create(&tid, NULL,tfn,(void *)&i);
         if (ret != 0)
         {
            perror("pthread_create failed");
         }
     }
     sleep(i+1);  //sleep睡眠一秒好执行 子线程
     return 0;
}