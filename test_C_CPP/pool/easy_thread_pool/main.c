 #include <unistd.h>
 #include <stdio.h>
 #include <stdlib.h>

 #include "thread_pool.h"

 void *func(void *arg) 
 {
     printf("create task_wokers %d\n", *(int *)arg);
     return NULL;
 }

 //入口函数
 int main()
 {
       //1.创建线程池
       if (tpool_create(5)!=0)
       {
          printf("tpool_create failed\n");
          exit(1);
       }
       
       int i,num[10];
       // 投递任务
       for ( i = 0; i < 10; i++)
       {    
            num[i] = i;  //用地址传值
            //printf("i adress is %p\n", &num[i]);
            //投递任务
            tpool_add_worker(func,(void *)&num[i]);
       }
       sleep(2);
       //销毁线程池
       tpool_destroy();
       return 0;
 }