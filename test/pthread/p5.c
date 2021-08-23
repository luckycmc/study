#include <stdio.h>

/**
  pthread_detach()函数设置线程分离  线程终止，会自动回收 pcb 无需回收
  检查错误描述fprintf(stderr,"XXX error:%s\n",strerror(ret)); ret 是错误号


  线程控制语句                             进程控制语句
  pthread_create()                        fork()
  pthread_self()                          getpid()
  pthread_exit()                          exit()
  pthread_join()                          wait/waitpid()
  pthread_cancel()                        kill
  pthread_detach
*/
int main()
{

}