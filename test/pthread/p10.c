#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>


//创建读写锁
pthread_rwlock_t rwlock; 

int data = 1;
//读锁  
void *readerA()
{
     while(1)
     {
            pthread_rwlock_rdlock(&rwlock);
            printf("A读者读出:%d\n",data);
            pthread_rwlock_unlock(&rwlock);
            sleep(1);//延时两秒
     }
}
//写入的事件
void *writerB()
{

    while(1){

        pthread_rwlock_wrlock(&rwlock);

        data++; // 数据加1

        printf("B作者写入:%d\n",data);

        pthread_rwlock_unlock(&rwlock);

        sleep(3);

    }

}
//主函数
int main()
{
    pthread_t t1;
    pthread_t t2;
    
    //读写锁初始化
    pthread_rwlock_init(&rwlock,NULL);
    //创建线程
    pthread_create(&t1, NULL,readerA,NULL);
    pthread_create(&t2, NULL,writerB,NULL);
    //回收线程
    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
    //释放锁
    pthread_rwlock_destroy(&rwlock);
    return 0;
}

