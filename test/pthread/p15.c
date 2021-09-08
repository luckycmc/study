#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#define _LOCK_
/****
 * 
 * 比如多个cpu上运行的任务之间，cpu运行的任务和中断之间，中断和中断之间，都有可能并发。

有些调度的概率虽然很小，但是不代表不发生，而且由于资源同步互斥导致的问题，很难复现，
纵观Linux内核代码，所有的临界资源都会对应锁。
*/

unsigned int value1,value2, count=0;

pthread_mutex_t mutex;  //创建互斥量

void *function(void *arg);

int main()
{
    pthread_t a_thread;

    //互斥量初始化
    if (pthread_mutex_init(&mutex, NULL) <0) //创建失败
    {
        perror("fail to mutex_init");
        exit(-1);
    }
    
    //创建子线程
    if (pthread_create(&a_thread, NULL,function,NULL) <0)  //线程创建失败做出对应的提示
    {
         perror("fail to create pthread_create");
         exit(-1);
    }
    
    while(1)
    {
        count++; 
#ifdef _LOCK_
        pthread_mutex_lock(&mutex);
#endif 
        value1 = count;
        value2 = count;
#ifdef _LOCK_
       pthread_mutex_unlock(&mutex);
#endif // _LOCK_
        
        return 0;
    }
}
//子线程执行空间
void *function(void *arg)
{
    while(1){

#ifdef _LOCK_
    pthread_mutex_lock(&mutex);
#endif // _LOCK_
        if (value1==value2)
        {
            printf("count=%d , value1=%d, value2=%d\n",  count, value1, value2);
            usleep(100000);
        }
#ifdef _LOCK_
    pthread_mutex_unlock(&mutex);
#endif // _LOCK_       
    }
    return NULL;
}

