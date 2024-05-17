//int pthread_join(pthread_id tid,void **retavl)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

struct thrd{
    int var;
    char str[256];
};

void sys_err(const char *str)
{
    perror(str);
    exit(1);
}
//线程操作的函数
void *tfn(void *arg)
{
    struct thrd *tval;
    tval = malloc(sizeof(struct thrd));//申请内存
    tval->var = 100;
    strcpy(tval->str,"hello thread");

    return (void *)tval;//强转空类型的指针
}
//主函数
int main()
{

     pthread_t tid;
     struct thrd *retval;

     int ret = pthread_create(&tid, NULL,tfn,NULL);
     if (ret != 0) 
     {
        sys_err("pthread create failed");
     }
     ret = pthread_join(tid,(void **)&retval);
     if (ret != 0) 
     {
        sys_err("pthread_join failed");
     }
     printf("children thread exit with var =%d,str=%s\n",retval->var,retval->str);
     //回收线程
     pthread_exit(NULL);
     return 0;
}