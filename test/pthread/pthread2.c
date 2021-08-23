#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

int var = 100;

//错误信息打印
void sys_err(const char *str)
{
    perror(str);
    exit(1);
}

//进程执行的函数

void *tfn(void *arg)
{   
    var =200;
    printf("thread,var = %d\n", var);
    return NULL;
}
//主函数
int main()
{
   printf("At first var = %d\n", var);
   pthread_t pid;
   int ret ;
   ret = pthread_create(&pid, NULL,tfn,NULL); //执行成功ret 等于0 
   if (ret != 0)
   {
       sys_err("create thread failed\n");
   }
   
   printf("result is %d\n", ret);
   sleep(1);
   printf("after pthread_create, var = %d\n", var);
   return 0;
}