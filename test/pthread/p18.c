#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * 线程的读写锁
 * 锁只有一把  以读数据的方式枷锁---读锁。以写的方式枷锁---写锁
 * 读时共享  写时独占
 * 写锁的优先级高
 * 相对应互斥量而言 读线程多的时候回提高效率
 * 
 */
// /* 3 个线程不定时 "写" 全局资源，5 个线程不定时 "读" 同一全局资源 */
int counter;

//创建读写锁
pthread_rwlock_t rwlock;

//写线程操作空间
void *th_write(void *arg)
{
    int t;
    int i = *(int *)arg;
    while(1)
    {
         t = counter; // 保存写之前的值
         usleep(1000);
         //加读锁
         pthread_rwlock_wrlock(&rwlock);
         printf("=======write %d: %lu: counter=%d ++counter=%d\n", i, pthread_self(), t, ++counter);
         //解锁
         pthread_rwlock_unlock(&rwlock);
         usleep(9000);   //给读锁提供读取的机会
    }
    return NULL;
}
// 读线程操作空间
void *th_read(void *arg)
{
    int i = *(int *)arg;
    while (1)
    {
            pthread_rwlock_rdlock(&rwlock);
            printf("----------------------------read %d: %lu: %d\n", i, pthread_self(), counter);
            pthread_rwlock_unlock(&rwlock);
            usleep(2000);   //给写锁提供机会
    }
    return NULL; //返回地址空间
}
//主函数
int main()
{
     int i;
     pthread_t tid[8];
     //初始化读写锁
     pthread_rwlock_init(&rwlock, NULL);
     
     //创建写线程
     for ( i = 0; i < 3; i++)
     {
          pthread_create(&tid[i], NULL,th_write,(void *)&i);  //&i取出对应的地址
     }
     //创建读线程
     for ( i = 0; i <5; ++i)
     {
         pthread_create(&tid[i+3], NULL,th_read,(void *)&i); //&i取出对应的地址
     }

     //回收子线程
     for ( i = 0; i <8;i++)
     {
         pthread_join(tid[i], NULL);
     }
     //释放读写锁
     pthread_rwlock_destroy(&rwlock);
     return 0; //
}