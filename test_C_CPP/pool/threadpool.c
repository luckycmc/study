

//工作者
typedef struct NWORKER{
    
      pthread_t threadid; //线程id
      int       terminate; //线程的状态

      struct NWORKER *next;  //链表的下一个节点
      struct NWORKER *prev;   //链表的上一个节点

}nWorker;

//任务
typedef struct NJOB{
    
     void(*func)(void *arg); //回调函数
     void *user_data;        // 对应的数据
 
     struct NJOB *prev; 
     struct NJOB *next;

}nJob;
//管理者
typedef struct MANAGER
{
    nWorker *wokers;
    nJob    *jobs;
    pthread_mutex_t *jobs_mtx;   //互斥锁
    pthread_cond_t  *jobs_cond;  // 条件变量
};


//取出任务

//创建线程池
int nThreadPoolCreate(nT){

}