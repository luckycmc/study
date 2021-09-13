 #include <unistd.h>
 #include <stdio.h>
 #include <stdlib.h>

 #include "thread_pool.h"

 void *func(void *arg) 
 {
     printf("thread %d\n", *(int *)arg);
     return NULL;
 }

 //入口函数
 int main()
 {
       //创建线程池
       if (tpool_create(5)!=0)
       {
          printf("tpool_create failed\n");
          exit(1);
       }
       int i;
       // 投递任务
       for ( i = 0; i < 10; i++)
       {
            tpool_add_worker(func,(void *)&i);
       }
       sleep(2);
       //销毁线程池
       tpool_destroy();
       return 0;
 }