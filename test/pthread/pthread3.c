#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
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
//线程回调执行的函数
void *ftn(void *arg)
{
    struct thrd *tval;
    //申请内存空间
    tval = malloc(sizeof(tval));
    tval->var = 100;
    strcpy(tval->str,"hello thread");
    return (void *)tval;
    
}
//主函数
int main()
{
    pthread_t tid;
    struct thrd *retval;

    //创建线程
    int ret = pthread_create(&tid,NULL,ftn,NULL);
    if (ret != 0)
    {
         sys_err("pthread_create error");
    }
    //int pthread_join(pthread_t thread, void **retval);
   ret = pthread_join(tid, (void **)&retval);
   if (ret != 0)
      sys_err("pthread_join error");

   printf("child thread exit with var= %d, str= %s\n", retval->var, retval->str);
   pthread_exit(NULL);
   return 0;

}