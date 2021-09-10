/*借助条件变量模拟 生产者-消费者 问题*/
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>

//链表作为公共区域需要呗互斥锁保护
struct msg
{
    struct msg *next;
    int num;
};

//创建头节点
struct msg *head;

/*****静态初始化一个条件变量和一个互斥量******/

pthread_cond_t has_product = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock       = PTHREAD_MUTEX_INITIALIZER;

//消费者模型
void *consumer(void *p)
{
    struct msg *mp;
    
    for ( ;;)
    {
        pthread_mutex_lock(&lock);
        //头节点为空说明没有节点数据 必须阻塞在这
        while (head == NULL){   //数据为空的时候阻塞等待
             
             //解锁并进行 阻塞等待
             pthread_cond_wait(&has_product,&lock);
        }
        //辅助节点
        mp = head;
        head = mp->next;   //模拟消费掉一个产品

        //解锁
        pthread_mutex_unlock(&lock);
        //打印消费信息
        printf("-Consume %lu---%d\n",pthread_self(),mp->num);
        //释放对应的节点
        free(mp);
        sleep(rand() % 5);
    }
    return NULL;
    
}
//生产者模型
void *producer(void *p)
{
    struct msg *mp;
    for ( ;;)
    {
        mp = malloc(sizeof(struct msg));
        mp->num = rand() %1000+1; //模拟生产一个产品
        printf("-Produce ---------------------%d\n", mp->num);
        //给共享数据枷锁
         pthread_mutex_lock(&lock);
        mp->next = head;
        head = mp;
        pthread_mutex_unlock(&lock);
        //在等待该条件变量上的一个线程唤醒
        pthread_cond_signal(&has_product);
        sleep(rand() % 5);
    }
    return NULL;
}

//主函数执行
int main()
{
    pthread_t pid,cid;
    srand(time(NULL));
    //创建生产和消费线程
    pthread_create(&pid, NULL,producer, NULL);
    pthread_create(&cid, NULL,consumer, NULL);
    //回收两个线程
    pthread_join(pid, NULL);
    pthread_join(cid, NULL);
    return 0;
}