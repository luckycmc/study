#include "./include/swoole.h"
#include "./include/Server.h"
#include "./include/RingQueue.h"
#include "./include/RingMempool.h"

typedef struct _swFactoryThread
{
	int writer_num;
	int writer_pti;
	swRingQueue *queues; //消息队列
	swRingMempool *pools; //缓存池
	swThreadWriter *writers;
} swFactoryThread;

static int swFactoryThread_writer_loop(swThreadParam *param);

int swFactoryThread_create(swFactory *factory, int writer_num)
{
	swFactoryThread *this;
	this = sw_calloc(writer_num, sizeof(swFactoryThread));
	if (this == NULL)
	{
		swTrace("malloc[0] fail\n");
		return SW_ERR;
	}
	this->writers = sw_calloc(writer_num, sizeof(swThreadWriter));
	if (this->writers == NULL)
	{
		swTrace("malloc[1] fail\n");
		return SW_ERR;
	}
	this->queues = sw_calloc(writer_num, sizeof(swRingQueue));
	if (this->queues == NULL)
	{
		swTrace("malloc[2] fail\n");
		return SW_ERR;
	}
	this->writer_num = writer_num;
	this->writer_pti = 0;

	factory->running = 1;
	factory->object = this;
	factory->dispatch = swFactoryThread_dispatch;
	factory->finish = swFactory_finish;
	factory->start = swFactoryThread_start;
	factory->shutdown = swFactoryThread_shutdown;

	factory->onTask = NULL;
	factory->onFinish = NULL;
	return SW_OK;
}

int swFactoryThread_start(swFactory *factory)
{
	swFactoryThread *this = factory->object;
	swThreadParam *param;
	int i;
	int ret;
	pthread_t pidt;

	ret = swFactory_check_callback(factory);
	if (ret < 0)
	{
		return SW_ERR;
	}
	for (i = 0; i < this->writer_num; i++)
	{
		ret = swPipeEventfd_create(&this->writers[i].evfd, 1);
		if (ret < 0)
		{
			swTrace("create eventfd fail\n");
			return SW_ERR;
		}
		param = sw_malloc(sizeof(swThreadParam));
		if (param == NULL)
		{
			return SW_ERR;
		}
		param->object = factory;
		param->pti = i;
		if (pthread_create(&pidt, NULL, (void * (*)(void *)) swFactoryThread_writer_loop, (void *) param) < 0)
		{
			swTrace("pthread_create fail\n");
			return SW_ERR;
		}
		if (swRingQueue_init(&this->queues[i], SW_QUEUE_SIZE) < 0)
		{
			swTrace("create ring queue fail\n");
			return SW_ERR;
		}
		this->writers[i].ptid = pidt;
		//SW_START_SLEEP;
	}
	return SW_OK;
}
int swFactoryThread_shutdown(swFactory *factory)
{
	swoole_running = 0;
	swFactoryThread *this = factory->object;
	free(this->writers);
	free(this);
	return SW_OK;
}
/**
 * 写线程模式
 */
int swFactoryThread_dispatch(swFactory *factory, swEventData *buf)
{
	swFactoryThread *this = factory->object;
	int pti;
	int ret;
	uint64_t flag = 1;
	int datasize = sizeof(int)*3 + buf->len;
	char *data;

	//使用pti，避免线程切换造成错误的writer_pti
	pti = this->writer_pti;
	if (this->writer_pti >= this->writer_num)
	{
		this->writer_pti = 0;
		pti = 0;
	}

	data = sw_malloc(datasize);
	if(data == NULL)
	{
		swTrace("malloc fail\n");
		return SW_ERR;
	}
	memcpy(data, buf, datasize);
	//send data ptr. use event_fd
	if (swRingQueue_push(&(this->queues[pti]), (void *) data) < 0)
	{
		swWarn("swRingQueue_push fail.Buffer is full.Writer=%d\n", pti);
		return SW_ERR;
	}
	else
	{
		ret = this->writers[pti].evfd.write(&this->writers[pti].evfd, &flag, sizeof(flag));
		if(ret < 0)
		{
			swWarn("Send queue notice fail.errno=%d\n", errno);
		}
		this->writer_pti++;
		return ret;
	}
}

static int swFactoryThread_writer_loop(swThreadParam *param)
{
	swFactory *factory = param->object;
	swServer *serv = factory->ptr;
	swFactoryThread *this = factory->object;
	int pti = param->pti;
	int ret;
	swEventData *req;
	uint64_t flag;

	//cpu affinity setting
	if (serv->open_cpu_affinity)
	{
		cpu_set_t cpu_set;
		CPU_ZERO(&cpu_set);
		CPU_SET(pti % SW_CPU_NUM, &cpu_set);
		if (0 != pthread_setaffinity_np(pthread_self(), sizeof(cpu_set), &cpu_set))
		{
			swTrace("pthread_setaffinity_np set fail\n");
		}
	}

	//main loop
	while (swoole_running > 0)
	{
		if (swRingQueue_pop(&(this->queues[pti]), (void **) &req) == 0)
		{
			factory->last_from_id = req->from_id;
			factory->onTask(factory, req);
			//swRingMempool_free(&this->queues[pti], req);
			sw_free(req);
		}
		else
		{
			ret = this->writers[pti].evfd.read(&this->writers[pti].evfd, &flag, sizeof(flag));
			if (ret < 0)
			{
				swTrace("read fail.errno=%d", errno);
			}
		}
	}
	factory->running = 0;
	//shutdown
	this->writers[pti].evfd.close(&this->writers[pti].evfd);
	sw_free(param);
	pthread_exit(SW_OK);
	return SW_OK;
}
factory, writer_pti, i);
				//printf("create process pid is %d\n",pid);
				if(pid < 0)
				{
					swTrace("Fork worker process fail.Errno=%d\n", errno);
					return SW_ERR;
				}
				else
				{
					this->workers[i].pid = pid;
				}
			}
			//管理进程进入事件循环
			swFactoryProcess_manager_loop(factory);
			break;
		default:
			for (i = 0; i < this->worker_num; i++)
			{   
				//printf("swFactoryProcess_manager_loop after\n");
				close(worker_pipes[i].pipes[1]); //关闭读通道
				writer_pti = (i % this->writer_num);
				/*************写进程的进程通讯fd 添加到reactor start**********/
				printf("writes pipi fd %d\n", worker_pipes[i].pipes[0]);
				this->writers[writer_pti].reactor.add(&(this->writers[writer_pti].reactor), worker_pipes[i].pipes[0], SW_FD_PIPE);
				this->workers[i].writer_id = writer_pti;
				this->workers[i].pipe_fd = worker_pipes[i].pipes[0];
				/*************写进程的进程通讯fd 添加到reactor start**********/
			}
			break;
		case -1:
			swTrace("[swFactoryProcess_worker_start]fork manager process fail\n");
			return SW_ERR;
	}
	return SW_OK;
}
//管理进程进入事件循环
static int swFactoryProcess_manager_loop(swFactory *factory)
{
	int pid, new_pid;
	int i, writer_pti;
	swFactoryProcess *this = factory->object;

	while(1)
	{
		pid = wait(NULL);
		for (i = 0; i < this->worker_num; i++)
		{
			if(pid != this->workers[i].pid) continue;

			writer_pti = (i % this->writer_num);
			new_pid = swFactoryProcess_worker_spawn(factory, writer_pti, i);
			if(new_pid < 0)
			{
				swTrace("Fork worker process fail.Errno=%d\n", errno);
				return SW_ERR;
			}
			else
			{
				this->workers[i].pid = new_pid;
			}
		}
	}
	return SW_OK;
}

static int swFactoryProcess_worker_spawn(swFactory *factory, int writer_pti, int worker_pti)
{
	swFactoryProcess *this = factory->object;
	int i, pid;

	pid = fork();
	if (pid < 0)
	{
		swTrace("[swFactoryProcess_worker_spawn]Fork Worker fail\n");
		return SW_ERR;
	}
	//worker child processor
	else if (pid == 0)
	{
		for (i = 0; i < this->worker_num; i++)
		{
			//非当前
			if(worker_pti!=i)
			{
				close(this->workers[i].pipe_fd);
			}
		}
		//工作进程进入事件循环
		swFactoryProcess_worker_loop(factory, this->workers[worker_pti].pipe_fd, worker_pti);
		exit(0);
	}
	//parent,add to writer
	else
	{
		return pid;
	}
}
//工作进程发送数据完毕
int swFactoryProcess_finish(swFactory *factory, swSendData *resp)
{
	//swFactoryProcess *this = factory->object;
	swEventData send_data;
	memcpy(send_data.data, resp->data, resp->len);
	send_data.fd = resp->fd;
	send_data.len = resp->len;
	
	return write(c_worker_pipe, &send_data, resp->len + (3 * sizeof(int)));
}
//工作进程进入事件循环 读取对应的数据
static int swFactoryProcess_worker_loop(swFactory *factory, int c_pipe, int worker_pti)
{  
	
	swEventData req;
	//swFactoryProcess *this = factory->object;
	swServer *serv = factory->ptr;
	c_worker_pipe = c_pipe;
	int n;
	int task_num = factory->max_request;
    /*****************绑定对应的cpu start****************/
	if (serv->open_cpu_affinity)
	{
		cpu_set_t cpu_set;
		CPU_ZERO(&cpu_set);
		CPU_SET(worker_pti % SW_CPU_NUM, &cpu_set);
		if (0 != sched_setaffinity(getpid(), sizeof(cpu_set), &cpu_set))
		{
			swTrace("pthread_setaffinity_np set fail\n");
		}
	}
    /*****************绑定对应的cpu end****************/
	//主线程
	while (swoole_running > 0 && task_num > 0)
	{
		n = read(c_pipe, &req, sizeof(req));
		swTrace("[Worker]Recv: pipe=%d|pti=%d\n", c_pipe, req.from_id);
		if (n > 0)
		{
			factory->last_from_id = req.from_id;
			factory->onTask(factory, &req);
			task_num--;
		}
		else
		{
			swTrace("[Worker]read pipe error.Errno=%d\n", errno);
		}
	}
	swTrace("[Worker]max request\n");
	return SW_OK;
}
//分发给对应的进程去处理
int swFactoryProcess_dispatch(swFactory *factory, swEventData *data)
{
	swFactoryProcess *this = factory->object;
	int pti = this->worker_pti;
	int ret;

	if (this->worker_pti >= this->worker_num)
	{
		this->worker_pti = 0;
		pti = 0;
	}
	swTrace("[ReadThread]sendto: pipe=%d|worker=%d\n", this->workers[pti].pipe_fd, pti);
	//send to unix sock
	ret = swWrite(this->workers[pti].pipe_fd, (char *)data, data->len + (3 * sizeof(int)));
	if(ret < 0)
	{
		return SW_ERR;
	}
	this->worker_pti++;
	return SW_OK;
}
//启动写的工作进程
static int swFactoryProcess_writer_start(swFactory *factory)
{
	swFactoryProcess *this = factory->object;
	swThreadParam *param;
	int i;
	pthread_t pidt;

	for (i = 0; i < this->writer_num; i++)
	{
		param = sw_malloc(sizeof(swThreadParam));
		if (param == NULL)
		{
			swTrace("malloc fail\n");
			return SW_ERR;
		}
		param->object = factory;
		param->pti = i;

		if (pthread_create(&pidt, NULL, (void * (*)(void *)) swFactoryProcess_writer_loop, (void *) param) < 0)
		{
			swTrace("pthread_create fail\n");
			return SW_ERR;
		}
		this->writers[i].ptid = pidt;
		SW_START_SLEEP;
	}
	return SW_OK;
}
//写进程接收对应的数据
int swFactoryProcess_writer_receive(swReactor *reactor, swEvent *ev)
{
	swFactory *factory = reactor->factory;
	swEventData resp;
	swSendData send_data;
	int n;

	//Unix Sock UDP
	n = read(ev->fd, &resp, sizeof(resp));
	swTrace("[WriteThread]recv: writer=%d|pipe=%d\n", ev->from_id, ev->fd);
	if (n > 0)
	{
		send_data.data = resp.data;
		send_data.len = resp.len;
		send_data.from_id = resp.from_id;
		send_data.fd = resp.fd;
		return factory->onFinish(factory, &send_data);
	}
	else if(n == 0){  //此时说明客户端已经关闭
        swTrace("client close [WriteThread]recv: writer=%d|pipe=%d\n", ev->from_id, ev->fd);
	}
	else
	{
		return SW_ERR;
	}
}
//进程写事件循环
int swFactoryProcess_writer_loop(swThreadParam *param)
{
	swFactory *factory = param->object;
	swFactoryProcess *this = factory->object;
	int pti = param->pti;
	swReactor *reactor = &(this->writers[pti].reactor);

	struct timeval tmo;
	tmo.tv_sec = 3;
	tmo.tv_usec = 0;

	reactor->factory = factory;
	reactor->id = pti;
	if (swReactorSelect_create(reactor) < 0)
	{
		swTrace("swReactorSelect_create fail\n");
		pthread_exit((void *)param);
	}
	reactor->setHandle(reactor, SW_FD_PIPE, swFactoryProcess_writer_receive);
	reactor->wait(reactor, &tmo);
	reactor->free(reactor);
	pthread_exit((void *)param);
	return SW_OK;
}

