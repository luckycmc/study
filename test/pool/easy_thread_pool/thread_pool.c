#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>  
#include <unistd.h>
 #include <stdio.h>

#include "thread_pool.h"

static tpool_t *tpool = NULL; //初始化线程池

/* 工作者线程函数, 从任务链表中取出任务并执行 */
static void *thread_routine(void *arg)
{
    tpool_worker_t *work;
    //执行对应的任务
    while (1)
    {
           /* 如果线程池没有被销毁且没有任务要执行，则等待 */
           //1. 枷锁
           pthread_mutex_lock(&tpool->queue_lock);

           //如果工作线程不为空,并且线程没有关闭 唤醒对应的线程
           while(!tpool->queue_head && !tpool->shutdown){
                
                 pthread_cond_wait(&tpool->queue_ready, &tpool->queue_lock);
           }
           //如果当前的线程状态是关闭的
           if (tpool->shutdown)
           {
                  //释放线程锁
                  pthread_mutex_unlock(&tpool->queue_lock);
                  //线程退出
                  pthread_exit(NULL);
           }
            work = tpool->queue_head;
            //链表
            tpool->queue_head = tpool->queue_head->next;
            pthread_mutex_unlock(&tpool->queue_lock);
            //具体执行函数
            work->routine(work->arg);
            free(work);
    }

    
}

//创建线程池
int tpool_create(int max_thr_num)
{
    int i;
    
    tpool = calloc(1, sizeof(tpool_t));
    if (!tpool)
    {
         printf("%s: calloc failed\n", __FUNCTION__);
         exit(1);
    }
    
    //对应属性的初始化
    tpool->max_thread_num = max_thr_num;
    tpool->shutdown   = 0; //默认为0
    tpool->queue_head = NULL;

    //初始化线程锁
    if (pthread_mutex_init(&tpool->queue_lock,NULL) != 0)
    {
        printf("%s: pthread_mutex_init failed, errno:%d, error:%s\n",__FUNCTION__, errno, strerror(errno));
        exit(1);
    }

    //条件变量初始化
    if (pthread_cond_init(&tpool->queue_ready,NULL)!=0)
    {
        printf("%s: pthread_cond_init failed, errno:%d, error:%s\n", __FUNCTION__, errno, strerror(errno));
          exit(1);
    }
    //创建工作线程池
    tpool->thr_t = calloc(max_thr_num,sizeof(pthread_t));
    
    if (!tpool->thr_t)
    {
        printf("%s: calloc failed\n", __FUNCTION__);
        exit(1);
    }

    //创建工作线程
    for (i = 0; i < max_thr_num; i++)
    {
          if (pthread_create(&tpool->thr_t[i], NULL,thread_routine, NULL) != 0)
          {
              printf("%s:pthread_create failed, errno:%d, error:%s\n", __FUNCTION__,errno, strerror(errno));
               exit(1);
          }
    }
     return 0;
}

/*销毁线程池*/
void tpool_destroy()
{
     int i;
     tpool_worker_t *member;
     if (tpool->shutdown)
     {
        return;
     }
     //关闭当前线程
     tpool->shutdown = 1;
    /* 通知所有正在等待的线程 */
    pthread_mutex_lock(&tpool->queue_lock);
    pthread_cond_broadcast(&tpool->queue_ready);
    pthread_mutex_unlock(&tpool->queue_lock);
     //线程回收
    
     for (i = 0; i < tpool->max_thread_num; ++i) 
     {
         pthread_join(tpool->thr_t[i], NULL);
     }

     free(tpool->thr_t);// 释放线程ID

     //释放所有的节点
     while(tpool->queue_head) {
         member = tpool->queue_head;
         tpool->queue_head = tpool->queue_head->next;
         free(member);
     }
     pthread_mutex_destroy(&tpool->queue_lock);//销毁线程锁
     //销毁条件变量
     pthread_cond_destroy(&tpool->queue_ready);
     free(tpool);

}
//向线程池 添加任务
int tpool_add_worker(void*(*routine)(void*), void *arg)
{   

    tpool_worker_t *work,*member;

     if (!routine){
         printf("%s:Invalid argument\n", __FUNCTION__);
         return -1;
     }
     work = malloc(sizeof(tpool_worker_t));
     work->routine = routine;
     work->arg = arg;
     work->next = NULL;
     pthread_mutex_lock(&tpool->queue_lock);
     member = tpool->queue_head;

     if (!member)  //节点没有数据了
     {
         tpool->queue_head = work;
     }else{
         while(member->next)
         {
            member = member->next;
         }
         member->next = work;
     }

      /* 通知工作者线程，有新任务添加 */
      pthread_cond_signal(&tpool->queue_ready);
      pthread_mutex_unlock(&tpool->queue_lock);

      return 0; 
     
}