#include "process_pool.h"
#include "worker.h"
#include "log.h"
#include "tinyswoole.h"
#include "server.h"

//worker进程的初始化 主要是申请内存 和管道的内存申请
int tswProcessPool_create(tswProcessPool *pool, int worker_num)
{    
    //worker进程分配 内存空间 
    pool->workers = (tswWorker *)malloc(sizeof(tswWorker) * worker_num);
    if (pool->workers == NULL) {
        tswWarn("%s", "malloc error");
        return TSW_ERR;
    }
    //管道分配内存空间
    pool->pipes = malloc(sizeof(tswPipe) * worker_num);
    if (pool->pipes == NULL) {
        tswWarn("%s", "malloc error");
		return TSW_ERR;
    }
    pool->worker_num = worker_num;

    return TSW_OK;
}
//worker进程的创建  主要任务是fork()创建进程  并设置 进程属性 进入事件循环
int tswServer_create_worker(tswServer *serv, tswProcessPool *pool, int worker_id)
{
    pid_t pid;
    tswWorker *worker;

    worker = pool->workers + worker_id;  //通过偏移量获取对应的worker 对象
    pid = fork();
    if (pid > 0) { // master process
        worker->pid = pid;
        worker->worker_id = worker_id;
        return TSW_OK;
    }

    // worker process 每一个进程一个 是相互独立的
    TSwooleWG.pipe_master = worker->pipe_master;
    TSwooleWG.pipe_worker = worker->pipe_worker;
    TSwooleWG.id = worker_id;
    // 工作进程进程事件循环
    if (tswWorker_loop() < 0) {
        tswWarn("%s", "tswWorker_loop error");
        return TSW_ERR;
    }
	
    return TSW_OK;
}
//进程池的相关信息
void tswProcessPool_info(const tswProcessPool *pool)
{
    tswWorker *worker = pool->workers;
    int i;
    for (i = 0; i < pool->worker_num; i++) {
        tswDebug("worker [%d] pid is %d", worker[i].worker_id, worker[i].pid);
    }
}