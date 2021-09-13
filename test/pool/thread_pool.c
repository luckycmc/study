#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>

#define DEFAULT_TIME 			60
#define MIN_WAIT_TASK_NUM  		10
#define DEFAULT_THREAD_VERY  	5
/**
   1.线程池模块分析
       创建线程池
       向线程池中添加数据，借助回调函数处理任务
       销毁线程池
    2.创建线程池结构体 指针
      初始化线程池结构体{N个 成员变量}

      创建 N个任务线程  // 任务线程主要处理 任务
      创建一个管理线程  // 管理线程 主要用于管理工作线程的 线程的创建和回收

      失败的时,销毁开辟的所有空间(释放)
    3.
       进入线程回调函数
       结束参数 void *arg -->pool 结构体
       枷锁---- lock 整个结构体枷锁
       判断条件变量---》wait
    4.
      循环10 秒执行一次
      进入管理者线程回调函数
      接受参数 void *arg ----> pool 结构体
      枷锁   ---- lock ----> 整个结构体枷锁
      获取线程池中用到 变量   task_num  live_num busy_num
      根据既定的算法 使用上述三个变量 判断是否是创建 销毁 指定部长的线程
*/
//  线程池是一种多线程处理形式，大多用于高并发服务器上，它能合理有效的利用高并发服务器上的线程资源；
/**
  在Unix网络编程中，线程与进程用于处理各项分支子功能，我们通常的操作是：接收消息 ==> 消息分类 ==> 线程创建 ==> 传递消息到子线程 ==> 线程分离 ==> 在子线程中执行任务 ==> 任务结束退出；
对大多数小型局域网的通信来说，上述方法足够满足需求；但当我们的通信范围扩大到广域网或大型局域网通信中时，我们将面临大量消息频繁请求服务器；在这种情况下，创建与销毁线程都已经成为一种奢侈的开销，特别对于嵌入式服务器来说更应保证内存资源的合理利用；
因此，线程池技术应运而生；线程池允许一个线程可以多次复用，且每次复用的线程内部的消息处理可以不相同，将创建与销毁的开销省去而不必来一个请求开一个线程；

结构讲解：
线程池是一个抽象的概念，其内部由任务队列，一堆线程，管理者线程组成；
*/
typedef struct {
    void *(*function)(void *);  /*函数指针，回调函数*/
    void *arg;                  /*上面函数的参数*/

} threadpool_task_t;            /*各子线程任务结构体*/  //子线程任务机构体  回调函数 和参数
/**
 * 
 * 线程池结构体 ，对线程池的基本定义
 */
typedef struct threadpool_s {

    pthread_mutex_t lock;            /*用于锁住本结构体*/
    pthread_mutex_t thread_counter;  /*记录忙状态线程个数的锁 -- busy_thread_num*/
    pthread_cond_t queue_not_full;   /*当队列任务满时，添加任务的线程阻塞，等待此条件变量*/
    pthread_cond_t queue_not_empty;  /*任务队列不为空时，通知等待任务的线程*/

    pthread_t *workers_tid;          /*存放线程池中每个线程的tid，数组*/ //记录整个线程的tid
    pthread_t manager_tid;           /*存管理线程tid*/ //  管理线程
    threadpool_task_t *task_queue;   /*任务线程队列*/

    int min_thread_num;                /*线程池最小线程数*/
    int max_thread_num;                /*线程池最大线程数*/
    int live_thread_num;               /*当前存活线程个数*/
    int busy_thread_num;               /*忙线程个数*/
    int wait_exit_thr_num;           /*要销毁的线程个数*/

    int queue_front;                 /*task_queue队头下表*/
    int queue_rear;                  /*task_queue队尾下表*/
    int queue_size;                  /*task_queue队列中实际任务数*/
    int queue_max_size;              /*task_queue队列可容纳任务上限*/

    int shutdown;                    /*标志位，线程池使用状态，true或者false*/

} threadpool_t;


//定义工作进程要做的事情
void *workers_thread( void *threadpool ); //线程池中工作线程要做的事情。
void *manager_thread(void *threadpool);  //管理线程要做的事情

/***************************************************************************
* 								创建线程池
*函数名: 
*		 threadpool_create()
*参  数:
*		 min_thread_num	    ：线程池中最小线程数量
*		 max_thread_num		：线程池中最大线程数量
*		 queue_max_size		：任务队列的最大长度
*作  用: 
*		 创建一个指定大小的线程池
*内  容:
*		1）线程池基本参数
*		2）工作线程
*		3）管理线程
*		4）任务队列
*		5）互斥锁、条件变量
****************************************************************************/
threadpool_t *threadpool_create(int min_thread_num, int max_thread_num, int queue_max_size)
{
    int i;
    threadpool_t *pool = NULL;
    
    do {
        pool = (threadpool_t *)malloc(sizeof(threadpool_t));//申请指针空间
        if (pool == NULL) {
            printf("malloc threadpool fail\n");
            goto err_1;
        }
        
        pool->min_thread_num  = min_thread_num; //最小个数
        pool->max_thread_num  = max_thread_num; // 最大个数
        pool->busy_thread_num = 0;
        pool->live_thread_num = min_thread_num; //活跃的线程个数
        pool->queue_size 	  = 0;
        pool->queue_max_size  = queue_max_size; //队列的长度
        pool->queue_front 	  = 0;
        pool->queue_rear 	  = 0;
        pool->shutdown        = 0;
        
        /*根据最大线程上限数，给工作线程数据开辟空间，并清零*/
        pool->workers_tid = (pthread_t *)malloc(sizeof(pthread_t) * max_thread_num);

        if (pool->workers_tid == NULL) {
            printf("malloc workers_tid fail\n");
            goto err_2;
        }
        memset(pool->workers_tid, 0, sizeof(pthread_t) * max_thread_num);
        
        /* 队列开辟空间 */
        pool->task_queue = (threadpool_task_t *)malloc(sizeof(threadpool_task_t) * queue_max_size);
        if (pool->task_queue == NULL) {
            printf("malloc task_queue fail\n");
            goto err_3;
        }
        
        /* 初始化互斥锁，条件变量 */
        if (pthread_mutex_init(&(pool->lock), NULL) != 0			|| 
            pthread_mutex_init(&(pool->thread_counter), NULL) != 0	|| 
            pthread_cond_init(&(pool->queue_not_empty), NULL) != 0	|| 
            pthread_cond_init(&(pool->queue_not_full), NULL)  != 0) {
                printf("init the lock or cond fail\n");
                goto err_4;
        }
        
        /* 启动 min_thread_num 个 work thread */
        for (i = 0; i < min_thread_num; i++) {
            //创建工作线程
            pthread_create(&(pool->workers_tid[i]), NULL, workers_thread,(void *)pool); /*pool指向当前线程池*/
            printf("start thread  0x%x...\n", (unsigned int)pool->workers_tid[i]);
        }
		/*创建管理者线程*/
        pthread_create(&(pool->manager_tid), NULL, manager_thread, (void *)pool);
        
    } while(0);
     
	return pool;
    //threadpool_free(pool);    /*前面代码调用失败，释放poll存储空间*/	
err_4:
	/*需要销毁互斥锁和条件变量*/
	free(pool->task_queue);	
err_3:
	free(pool->workers_tid);
err_2:
	free(pool);
err_1:	
    return NULL;
}
/*****************定义对应的参数*********************/

/***************************************************************************
* 								管理者线程
*函数名: 
*		manager_thread()
*参  数:
*		threadpool	: 使用的线程池
*作  用: 
*		 根据任务的数量动态调整线程池大小
*内  容:
*		1）获取当前线程池中存在的线程和任务队列中积累的任务
*		2）根据需求动态调整线程池中的线程的数量
*缺  点：
*		使用了太多的互斥锁和条件变量，效率上值得商榷
****************************************************************************/
void *manager_thread(void *threadpool)
{
    threadpool_t *pool = (threadpool_t *)threadpool;
    int i;
    //只要线程池存在 管理线程会一直管理 工作线程池
    while (!pool->shutdown)
    {

        sleep(DEFAULT_TIME);   /*定时对线程池管理*/
        
        pthread_mutex_lock(&(pool->lock));
        int queue_size = pool->queue_size;
        int live_thread_num = pool->live_thread_num;/*线程池中存在的线程数量*/
        pthread_mutex_unlock(&(pool->lock));
        
        pthread_mutex_lock(&(pool->thread_counter));
        int busy_thread_num = pool->busy_thread_num;
        pthread_mutex_unlock(&(pool->thread_counter));
        
        /* 创建新线程算法，任务数大于最小线程池个数，
         * 且存活的线程数小于最大线程数时
         */
        if (queue_size >= MIN_WAIT_TASK_NUM && live_thread_num < pool->max_thread_num)
        {
            pthread_mutex_lock(&(pool->lock));

            int add = 0;
            
            /* 一次增加 DEFAULT_THREAD_VERY 个线程*/
            for (i = 0; i < pool->max_thread_num && add < DEFAULT_THREAD_VERY && pool->live_thread_num < pool->max_thread_num; i++) {
                if (pool->workers_tid[i] == 0 || !is_thread_alive(pool->workers_tid[i])) 
                {   
                    // 创建工作线程
                    pthread_create(&(pool->workers_tid[i]), NULL,workers_thread,(void *)pool);
                    add++;
                    pool->live_thread_num++;  //活跃个数也要添加
                }
            }
            
            pthread_mutex_unlock(&(pool->lock));
        }
        
        /* 销毁多余的空闲线程算法，忙线程 x2 小于存活的线程数 且 
         * 存活的线程数大于最小线程数时
         */
        if (busy_thread_num * 2 < live_thread_num && live_thread_num > pool->min_thread_num) 
        {
            /*一次销毁 DEFAULT_THREAD_VERY 个线程*/
            pthread_mutex_lock(&(pool->lock));
            pool->wait_exit_thr_num = DEFAULT_THREAD_VERY;
            pthread_mutex_unlock(&(pool->lock));
            
            for (i = 0; i < DEFAULT_THREAD_VERY; i++)
            {
                /*通知处在空闲状态的线程，他们会自行终止，线程自杀*/
                pthread_cond_signal(&(pool->queue_not_empty));
            }
        }
    }
    
    return NULL;
}

/***************************************************************************
* 								工作线程处理函数
*函数名: 
*		 workers_thread()
*参  数:
*		 threadpool	    ：包含线程池中所有的参数
*作  用: 
*		 等待分配任务并执行之
*内  容:
*		1）睡眠等待分配任务
*		2）是否终结本线程
*		3）从任务队列上取任务，更改线程busy状态
*		4）执行任务
*		5）恢复为空闲状态
*缺  点：
*		使用了太多的互斥锁和条件变量，效率上值得商榷
****************************************************************************/
void *workers_thread(void *threadpool)
{
    threadpool_t *pool = (threadpool_t *)threadpool; //初始化工作池

    threadpool_task_t task; // 工作线程结构体

    while(1) 
    {
        /* Lock must be taken to wait on condition variable */
        /* 刚创建出线程，等待任务队列里面有任务，否则阻塞等待任务队列里有任务再唤醒
         * 接收任务
         */
        pthread_mutex_lock(&(pool->lock));
        
        /* queue_size == 0 说明没有任务，调wait 阻塞在条件变量上，若有任务，跳过该while */
        while((pool->queue_size == 0) && (!pool->shutdown)) 
        {
            printf("Workers'thread ID 0x%x is waiting\n", (unsigned int)pthread_self());
            pthread_cond_wait(&(pool->queue_not_empty), &(pool->lock));
            
            /* 清除指定数目的空闲线程，如果要结束的线程个数大于0，结束线程 */
            if (pool->wait_exit_thr_num > 0) 
            { 
                /* 如果线程池里的线程个数大于最小值时可以结束当前线程 */
                if (pool->live_thread_num > pool->min_thread_num) 
                {
                    printf("Workers'thread ID 0x%x is exiting\n", (unsigned int)pthread_self());
                    pool->live_thread_num--;
					pool->wait_exit_thr_num--;
                    pthread_mutex_unlock(&(pool->lock));
                    pthread_exit(NULL);
                }
            }
        }
        
        /*如果关闭了线程池，自行退出处理*/
        if (pool->shutdown == 1) {
			printf("Workers'thread ID 0x%x is exiting\n", (unsigned int)pthread_self());
            pthread_mutex_unlock(&(pool->lock));
            pthread_exit(NULL);
        }
        
        /*从任务队列里获取任务，是一个出队操作*/
        task.function = pool->task_queue[pool->queue_front].function;
        task.arg = pool->task_queue[pool->queue_front].arg;

        /*出队，模拟环形队列*/
        pool->queue_front = (pool->queue_front + 1) % pool->queue_max_size;
        pool->queue_size--;
        
        /*通知可以有新的任务添加进来*/
        pthread_cond_broadcast(&(pool->queue_not_full));
        
        /*任务取出后，立即将线程池锁释放*/
        pthread_mutex_unlock(&(pool->lock));
        
		/*设置当前线程忙状态*/
        pthread_mutex_lock(&(pool->thread_counter));       /*忙状态线程数变量锁*/
        pool->busy_thread_num++;                           /*忙状态线程数+1*/
        pthread_mutex_unlock(&(pool->thread_counter));
		
		/*执行任务*/
        (*(task.function))(task.arg);                  
        
        /*由忙状态切换为空闲状态*/
        pthread_mutex_lock(&(pool->thread_counter));
        pool->busy_thread_num--;
        pthread_mutex_unlock(&(pool->thread_counter));
    }
    
    return NULL;
}
/***************************************************************************
* 						向线程池的任务队列中添加一个任务
*函数名: 
*		threadpool_add()
*参  数:
*		pool		: 使用的线程池
*		function	：任务的执行函数
*		arg			：任务执行参数的参数
*作  用: 
*		 向线程池的任务队列中添加一个任务
*内  容:
*		1）任务队列是否已满
*		2）添加任务
*		3）唤醒在任务队列上睡眠的线程
*缺  点：
*		使用了太多的互斥锁和条件变量，效率上值得商榷
****************************************************************************/

int threadpool_add(threadpool_t *pool, void *function(void *arg), void *arg)
{
    pthread_mutex_lock(&(pool->lock));
    
    /*为真，队列已满，调wait等待*/
    while ((pool->queue_size == pool->queue_max_size) && (!pool->shutdown)) {
        pthread_cond_wait(&(pool->queue_not_empty), &(pool->lock));
    }
    
    if (pool->shutdown) {
        pthread_mutex_unlock(&(pool->lock));
		return 0;
    }
    
    /*清空工作线程 调用回调函数的参数 arg*/
    if (pool->task_queue[pool->queue_rear].arg != NULL) {
        free(pool->task_queue[pool->queue_rear].arg);
        pool->task_queue[pool->queue_rear].arg = NULL;
    }
    /*添加任务到任务队列里面*/
    pool->task_queue[pool->queue_rear].function = function;  // 回调函数 也就是处理函数
    pool->task_queue[pool->queue_rear].arg = arg;            // 对应的参数 
    pool->queue_rear = (pool->queue_rear + 1) % pool->queue_max_size;
    pool->queue_size++;
    
    /*添加完任务后，队列不为空，唤醒线程池中等待处理任务的线程*/
    pthread_cond_signal(&(pool->queue_not_empty));
    pthread_mutex_unlock(&(pool->lock));
    
    return 0;
}

/***************************************************************************
* 								销毁线程池
*函数名: 
*		threadpool_distory()
*参  数:
*		threadpool	: 要销毁的线程池
*作  用: 
*		 销毁线程池
*内  容:
*		1）发送任务，销毁线程
*		2）回收线程资源
*缺  点：
*		无
****************************************************************************/

int threadpool_distory(threadpool_t *pool)
{
    int i;
    if (pool == NULL) {
        return -1;
    }

    pool->shutdown = 1;
    
    /*先销毁管理线程*/
    pthread_join(pool->manager_tid, NULL);
    /*通知所有空闲线程*/
    for (i = 0; i < pool->live_thread_num; i++) 
    {   
        //销毁所有的空闲线程
        pthread_cond_broadcast(&(pool->queue_not_empty));
    }
    /*回收所有管理者线程资源*/
    for (i = 0; i < pool->live_thread_num; i++) 
    {
        pthread_join(pool->workers_tid[i], NULL);
    }
    // 内存池的释放
    threadpool_free(pool);
    return 0;
}

/***************************************************************************
* 								释放线程池资源
*函数名: 
*		threadpool_free()
*参  数:
*		threadpool	: 要释放的线程池
*作  用: 
*		 释放线程池资源
*内  容:
*		1）释放任务队列
*		2）销毁互斥锁和条件变量
*		3）释放线程池
*缺  点：
*		无
****************************************************************************/
int threadpool_free(threadpool_t *pool)
{
    if (pool == NULL) {
        printf("thread pool is already free\n");
        return -1;
    }
    
    if (pool->task_queue) {
        free(pool->task_queue);
    }
    
    if (pool->workers_tid)
     {
        free(pool->workers_tid);
        pthread_mutex_lock(&(pool->lock));
        pthread_mutex_destroy(&(pool->lock));
        pthread_mutex_lock(&(pool->thread_counter));
        pthread_mutex_destroy(&(pool->thread_counter));
        pthread_cond_destroy(&(pool->queue_not_empty));
        pthread_cond_destroy(&(pool->queue_not_full));
    }
    free(pool);
    pool = NULL;
    
    return 0;
}

/***************************************************************************
* 							判断当前线程是否存在
*函数名: 
*		is_thread_alive()
*参  数:
*		tid	: 待查询的线程PID
*作  用: 
*		 判断当前线程是否存在
*内  容:
*		1）发送0信号来判断
*缺  点：
*		无
****************************************************************************/
int is_thread_alive(pthread_t tid)
{    
    // pthread_kill(tid,0) //判断线程是否存活 如果存活则返回为1 不存活 返回为0 

    // ESRCH 表示线程不存在了
    int kill_rc = pthread_kill(tid, 0); /*发0号信号，测试线程是否存活*/
    if (kill_rc == ESRCH) {
        return 0;
    }   
    return 1;
}
/***************************************************************************
* 							以下为测试demo
*函数名: 
*		process()
*参  数:
*		arg	: 任务参数
*作  用: 
*		 任务处理函数
*内  容:
*		1）执行任务
*缺  点：
*		无
****************************************************************************/
// update user set authentication_string=password('dhw456123') where user='zs'
/* 线程池中的线程，模拟处理业务 */  // 这是线程池工作的回调函数
void process(void *arg)
{
    printf("thread 0x%x working on task %d\n", (unsigned int)pthread_self(),
        *(int *)arg);
    sleep(1);
    printf("task %d is end\n", *(int *)arg);
}
//主函数执行
int main(int argc, char **argv)
{
    int num[20], i;
    /*threadpool_t *threadpool_create(int min_thread_num, int max_thread_num, int queue_max_size)*/
    
    /* 创建线程池，池里最小3个线程，最大100，队列最大100 */
    threadpool_t *thp = threadpool_create(3, 100, 100);
    if (thp == NULL) {
        printf("threadpool_create fail\n");
        return 0;
    }
    printf("pool init\n");
    
    for (i = 0; i < 20; i++) {
        num[i] = i;
        printf("add task %d\n", i);
        /* 向线程池中添加任务 */
        threadpool_add(thp, (void *)&process, (void *)&num[i]);
    }
    /*等待子线程完成任务*/
	//最好做个线程回收

	sleep(30);
	//释放内存池
    threadpool_distory(thp);
    
    return 0;
}