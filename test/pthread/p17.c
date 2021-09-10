#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
/****
 主线程和子线程在访问共享区时就没有交叉输出的情况了
 尽量保证锁的粒度， 越小越好。（访问共享数据前，加锁。访问结束【立即】解锁。）
 互斥锁，本质是结构体。 我们可以看成整数。 初值为 1。（pthread_mutex_init() 函数调
用成功。）
加锁： --操作， 阻塞线程。
解锁： ++操作， 唤醒阻塞在锁上的线程。
try 锁：尝试加锁，成功--。失败，返回。同时设置错误号 EBUSY

 * */
//定义一把锁
pthread_mutex_t mutex;
/****
 输出结果是主线程和子线程交叉的,因为产生的线程切换 导致数据输出不一样
 * */
//线程的执行函数
void *tfn()
{
    srand(time(NULL));
    while (1){
        pthread_mutex_lock(&mutex);//枷锁
        printf("hello");
        sleep(rand()%3); /*模拟长时间操作共享资源，导致 cpu 易主，产生与时间有关的错误*/
        printf("world\n");
        pthread_mutex_unlock(&mutex);//解锁
        sleep(rand()%3);
    }
    return NULL;
}
//主函数
int main()
{
     pthread_t tid;
     srand(time(NULL));
     //初始化话锁
     pthread_mutex_init(&mutex, NULL);
     //创建线程
     pthread_create(&tid,NULL,tfn,NULL);

     while(1){
         pthread_mutex_lock(&mutex); // 加锁
         printf("HELLO ");
         sleep(rand() % 3);
         printf("WORLD\n");
         pthread_mutex_unlock(&mutex);
         sleep(rand() % 3);
     }
     pthread_join(tid, NULL);
     //销毁互斥锁
     pthread_mutex_destroy(&mutex);//销毁互斥锁
     return 0;
}

