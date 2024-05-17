#ifndef PROCESS_POOL_H_
#define PROCESS_POOL_H_

#include "tinyswoole.h"
// wokrker 进程
struct _tswWorker {
	pid_t pid;
	int worker_id;
	int pipe_master;   //reactor  线程 通讯fd
	int pipe_worker;   //worker   线程 通讯fd
	tswPipe *pipe_object;
};
//进程池的相关信息
struct _tswProcessPool {
	int worker_num;
	tswPipe *pipes;
	tswWorker *workers;
};

/*
  struct _tswPipe {
	void *object;

	int (*read)(tswPipe *pipe, void *recv, int length);
    int (*getFd)(tswPipe *pipe, int isWriteFd);
	int (*write)(tswPipe *pipe, void *send, int length);
};

*/

int tswProcessPool_create(tswProcessPool *pool, int worker_num);
int tswServer_create_worker(tswServer *serv, tswProcessPool *pool, int worker_id);
void tswProcessPool_info(const tswProcessPool *pool);

#endif /* PROCESS_POOL_H_ */
