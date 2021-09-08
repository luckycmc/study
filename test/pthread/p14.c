#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
/****
 * 线程的调度是随机的  因此存在则很大的不确定性
 * 
  调度的最小单位是轻量级进程【比如我们编写的hello world最简单的C程序，执行时就是一个轻量级进程】或者线程；
  每个线程都会分配一个时间片，时间片到了就会执行下一个线程；
  线程的调度有一定的随机性，无法确定什么时候会调度；
  在同一个进程内，创建的所有线程除了线程内部创建的局部资源，进程创建的其他资源所有线程共享；
  比如：主线程和子线程都可以访问全局变量，打开的文件描述符等。
 **/
unsigned int value1,value2,count = 0;

void *function(void *arg);

//主函数
int  main()
{
     pthread_t a_thread;
     //创建子线程执行子线程
     if(pthread_create(&a_thread, NULL, function,NULL))
     {
          perror("fail to create thread");
          exit(-1);
     }
     //执行对应的数据操作
     while(1)
     {
           count++;
           value1 = count;
           value2 = count;
     }

}
//子线程执行空间
void *function(void *arg)
{
    while(1){
        if (value1==value2)
        {
           printf("count=%d , value1=%d, value2=%d\n",  count, value1, value2);
            usleep(100000);
        }
        
    }
    return NULL;
}