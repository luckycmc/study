#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

/*************
 * pthread_cond_signal(): 唤醒阻塞在条件变量上的 (至少)一个线程。
 * pthread_cond_broadcast()： 唤醒阻塞在条件变量上的 所有线程。
 * */
//信号量概念及其相关操作函数
/*
   信号量:
      应用于线程,进程间的同步
      相当于 初始化值为 N 的互斥量。 N 值，表示可以同时访问共享数据区的线程数。
      函数：
            sem_t sem; 定义类型。
            int sem_init(sem_t *sem, int pshared, unsigned int value);
        参数：
            sem： 信号量
            pshared： 0： 用于线程间同步
        1： 用于进程间同步
                value：N 值。（指定同时访问的线程数）
                sem_destroy();
                sem_wait(); 一次调用，做一次-- 操作，当信号量的值为 0 时，再次 -- 就会阻塞。
                （对比 pthread_mutex_lock）
            sem_post(); 一次调用，做一次++ 操作. 当信号量的值为 N 时, 再次 ++ 就会阻塞。
            （对比 pthread_mutex_unlock）
*/
/*信号量实现 生产者 消费者问题*/
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>

#define NUM 5

int queue[NUM];    //全局数组实现环形对队列

sem_t blank_number,product_number; //空格信号量，产品信号量

//生产者
void *producer(void *arg)
{
     int i = 0; 
     while (1)
     {
         sem_wait(&blank_number);// //生产者将空格子数--,为 0 则阻塞等待
         queue[i] = rand() % 1000 + 1; //生产一个产品
         printf("----Produce---%d\n", queue[i]);
         sem_post(&product_number);// 将产品数加一

         i = (i+1) % NUM; //借助下标实现环形
         sleep(rand()%1);
     }
     return NULL;
}
/**
 * 消费者
 * 
 * @param arg 
 * @return void* 
 */
void *consumer(void *arg)
{
    int i= 0;
    while (1){
        sem_wait(&product_number); //消费者将产品数--,为 0 则阻塞等待
        printf("-Consume---%d\n", queue[i]);
        queue[i] = 0; //消费一个产品
        sem_post(&blank_number); //消费掉以后,将空格子数++
        i = (i+1) % NUM;
        sleep(rand()%3);
    }

    return NULL; //
}
//主函数
int main()
{
     pthread_t pid, cid;
     sem_init(&blank_number,0,NUM);    //初始化空格子信号量为 5, 线程间共享 -- 0
     sem_init(&product_number,0,0);        //产品数为 0

     pthread_create(&pid, NULL, producer, NULL);
     pthread_create(&cid, NULL, consumer, NULL);

     pthread_join(pid, NULL);
     pthread_join(cid, NULL);

     sem_destroy(&blank_number);
     sem_destroy(&product_number);
     return 0;
} 