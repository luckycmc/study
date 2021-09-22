#ifndef THREAD_POOL_H__
#define THREAD_POOL_H__

#include <pthread.h>

/*******要执行任务的列表********/
typedef struct tpool_worker{
      
      void* (*routine)(void*);   /*任务函数*/
      void* arg; // 参数
      struct tpool_worker *next; // 任务的下一个节点 形成一个链表

}tpool_worker_t;

//线程池结构体
typedef struct tpool{
    
     int shutdown;          //线程是否销毁

     int max_thread_num;    //最大线程数

     pthread_t *thr_t;      //线程id 数组

     tpool_worker_t *queue_head; //工作任务队列的表头

     //初始化线程锁
     pthread_mutex_t queue_lock; //线程锁
     //初始化条件变量  
     pthread_cond_t  queue_ready;  //条件变量

}tpool_t;

// 创建线程池 max_thr_num 最大线程数
int tpool_create(int max_thr_num);

//销毁线程池
void tpool_destroy();

//向线程池中添加任务
int tpool_add_worker(void*(*routine)(void*), void *arg);

#endif // THREAD_POOL_H__