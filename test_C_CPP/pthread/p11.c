#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <error.h>
#include <pthread.h>
/** 
    pthread_cond_signal(): 唤醒阻塞在条件变量上的 (至少)一个线程。
    pthread_cond_broadcast()： 唤醒阻塞在条件变量上的 所有线程。
 * */
//创建一个全局变量
struct msg{
     
      int num;
      struct msg *next;
};
struct msg *head; //头指针

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; //定义初始化互斥量
pthread_cond_t  has_data = PTHREAD_COND_INITIALIZER; //定义初始化互斥量
//错误打印函数
void err_thread(int ret,char *str)
{
    if(ret != 0){
        
        fprintf(stderr,"%s:%s",str,strerror(ret));
        pthread_exit(NULL);
    }
}
//生产者
void *produser(void *arg)
{   
    while(1){
          
          
        struct msg *mp = malloc(sizeof(struct msg));
        mp->num = rand() % 1000 +1;
        //打印输出对应的数据
        printf("--produce %d\n", mp->num); 

        //给线程枷锁
        pthread_mutex_lock(&mutex);  //枷锁 互斥量
        mp->next = head;             //数据的公共区域
        head = mp;
        //线程解锁
        pthread_mutex_unlock(&mutex);// 解锁  互斥量
        
        // 唤醒 阻塞在条件遍变量 has_data 上的线程
        pthread_cond_signal(&has_data);

        sleep(rand() %3);

    }
    return NULL;
}
//消费者
void *consumer(void *arg)
{
    while(1){
         
          struct msg *mp;
          pthread_mutex_lock(&mutex);//加上锁互斥锁
          if (head == NULL)
          {
              pthread_cond_wait(&has_data, &mutex); //阻塞等待条件变量
          }
          // // pthread_cond_wait 返回时, 重写加锁 mutex
          mp = head;
          head =mp->next;//节点指向改变下移

          //解锁
          pthread_mutex_unlock(&mutex);
          printf("---------------consumer:%d\n",mp->num);
          free(mp);  //释放链表
          sleep(rand() % 3);
    }
    return NULL;
}
//主函数
int  main()
{
     int ret;
     pthread_t pid,cid;

     srand(time(NULL)); //产生随机数

     ret = pthread_create(&pid,NULL,produser,NULL);//生产者
     if(ret != 0){
         
           err_thread(ret,"pthread_create error");
     }
     
     ret = pthread_create(&cid,NULL,consumer,NULL);// 消费者
     if(ret != 0){
         
           err_thread(ret,"pthread_create error");
     }
     //回收线程
     pthread_join(pid,NULL);
     pthread_join(cid,NULL);

     return 0;
}