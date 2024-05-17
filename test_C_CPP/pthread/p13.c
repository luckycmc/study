/*信号量实现 生产者 消费者问题*/
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>

#define NUM 5

int queue[NUM]; //全局数组实现环形对列

sem_t blank_number,product_number; //空格子信号量，产品信号量
//生产
void *producer(void *arg)
{

}
//消费
void consumer(void *arg)
{

}
//主函数
int main()
{
    
}