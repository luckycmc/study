#include <time.h>
#include <sys/timerfd.h>
#include <sys/socket.h>

#include "./include/swoole.h"
#include "./include/Server.h"

static void swSignalInit(void);
static int swServer_poll_loop(swThreadParam *param);
static int swServer_poll_start(swServer *serv);
static int swServer_check_callback(swServer *serv);
static int swServer_listen(swServer *serv, swReactor *reactor);
static int swServer_poll_onClose(swReactor *reactor, swEvent *event);
static int swServer_poll_onReceive(swReactor *reactor, swEvent *event);
static int swServer_poll_onPackage(swReactor *reactor, swEvent *event);
static int swServer_timer_start(swServer *serv);

char swoole_running = 1;
uint16_t sw_errno = 0;
char sw_error[SW_ERROR_MSG_SIZE];
//服务端关闭连接时触发函数
int swServer_onClose(swReactor *reactor, swEvent *event)
{
	swServer *serv = reactor->ptr;
	swEventClose cev;
	swReactor *from_reactor;
	int ret;
	ret = serv->main_pipe.read(&serv->main_pipe, &cev, sizeof(uint64_t));
	if (ret < 0)
	{
		return SW_ERR;
	}
	swTrace("Close Event.fd=%d|from=%d\n", cev.fd, cev.from_id);
	serv->onClose(serv, cev.fd, cev.from_id);
	from_reactor = &(serv->poll_threads[cev.from_id].reactor);
	from_reactor->del(from_reactor, cev.fd);
	ret = close(cev.fd);
	return ret;
}

int swServer_onTimer(swReactor *reactor, swEvent *event)
{
	swServer *serv = reactor->ptr;
	uint64_t exp;
	int ret;
	swTimerList_node *timer_node;
	time_t now;

	time(&now);
	ret = read(serv->timer_fd, &exp, sizeof(uint64_t));
	if (ret < 0)
	{
		return SW_ERR;
	}
	LL_FOREACH(serv->timer_list, timer_node)
	{
		if (timer_node->lasttime < now - timer_node->interval)
		{
			serv->onTimer(serv, timer_node->interval);
			timer_node->lasttime += timer_node->interval;
		}
	}
	swTrace("Timer Call");
	return ret;
}
//接收客户端了解请求
int swServer_onAccept(swReactor *reactor, swEvent *event)
{
	swServer *serv = reactor->ptr;
	int clilen;
	int conn_fd;
	int ret;
	char *str;

	struct sockaddr_in clientaddr;
	clilen = sizeof(clientaddr);
	bzero(&clientaddr, clilen);
	//swTrace("[Main]accept start\n");
	//得到连接套接字
	while (1)
	{
#ifdef SW_USE_ACCEPT4
		conn_fd = accept4(event->fd, (struct sockaddr *) &clientaddr, (socklen_t *) &clilen, SOCK_NONBLOCK);
#else
		conn_fd = accept(event->fd, (struct sockaddr *) &clientaddr, (socklen_t *) &clilen);
#endif
		if (conn_fd < 0)
		{
			//中断
			if(errno == EINTR)
			{
				continue;
			}
			else
			{
				swTrace("[Main]accept fail Errno=%d|SockFD=%d|\n", errno, event->fd);
				return SW_ERR;
			}
		}
#ifndef SW_USE_ACCEPT4
		swSetNonBlock(conn_fd);
#endif
		break;
	}

	//TCP Nodelay
	if(serv->open_tcp_nodelay == 1)
	{
		int flag = 1;
		setsockopt(conn_fd, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(flag));
	}

	str = inet_ntoa(clientaddr.sin_addr);
	swTrace("[Main]connect from %s, by process %d\n", str, getpid());

	if (serv->c_pti >= serv->poll_thread_num)
	{
		serv->c_pti = 0;
	}
    /*
	  对应的accept fd加入到对应的epoll 中 表示有新的连接到来时的情况 会触发对应的
	*/ 
	ret = serv->poll_threads[serv->c_pti].reactor.add(&(serv->poll_threads[serv->c_pti].reactor), conn_fd, SW_FD_TCP);
	if (ret < 0)
	{
		swTrace("[Main]add event fail Errno=%d|FD=%d\n", errno, conn_fd);
		return SW_ERR;
	}
	/*********表示连接成功了触发注册的连接回调事件 start************/ 
	serv->onConnect(serv, conn_fd, serv->c_pti);
	/*********表示连接成功了触发注册的连接回调事件 end************/ 
	serv->c_pti++;
	return SW_OK;
}

int swServer_addTimer(swServer *serv, int interval)
{
	swTimerList_node *timer_new = sw_malloc(sizeof(swTimerList_node));
	time_t now;
	time(&now);
	if (timer_new == NULL)
	{
		swWarn("malloc fail\n");
		return SW_ERR;
	}
	timer_new->lasttime = now;
	timer_new->interval = interval;
	LL_APPEND(serv->timer_list, timer_new);
	if (serv->timer_interval == 0 || interval < serv->timer_interval)
	{
		serv->timer_interval = interval;
	}
	return SW_OK;
}

void swServer_timer_free(swServer *serv)
{
	swTimerList_node *node;
	LL_FOREACH(serv->timer_list, node)
	{
		LL_DELETE(serv->timer_list, node);
		sw_free(node);
	}
	close(serv->timer_fd);
}
//检查回调函数
static int swServer_check_callback(swServer *serv)
{
	if (serv->onStart == NULL)
	{
		return SW_ERR;
	}
	if (serv->onConnect == NULL)
	{
		return SW_ERR;
	}
	if (serv->onReceive == NULL)
	{
		return SW_ERR;
	}
	if (serv->onClose == NULL)
	{
		return SW_ERR;
	}
	if (serv->onShutdown == NULL)
	{
		return SW_ERR;
	}
	if (serv->timer_list != NULL && serv->onTimer == NULL)
	{
		return SW_ERR;
	}
	return SW_OK;
}
/************************swoole server 启动*****************************/
int swServer_start(swServer *serv)
{
	swReactor main_reactor;
	swFactory *factory = &serv->factory;

	struct timeval tmo;
	int ret;
    //检验对应注册的回调函数
	ret = swServer_check_callback(serv);
	if (ret < 0)
	{
		swError("Callback function is null.");
		return ret;
	}
	//run as daemon
	if (serv->daemonize > 0)  //守护进程模式
	{ 
		if (daemon(0, 0) < 0)
		{
			return SW_ERR;
		}
	}
	/*************启动整个个服务于的进程 start*******/
	ret = factory->start(factory);
	if (ret < 0)
	{
		return SW_ERR;
	}
	/*************启动整个个服务于的进程 start*******/
	//启动事件循环
	ret = swServer_poll_start(serv);
	if (ret < 0)
	{
		return SW_ERR;
	}

	SW_START_SLEEP;
	//ret = swReactorSelect_create(&main_reactor);
    //主线程创建reactor 模型
	ret = swReactorPoll_create(&main_reactor, 10); //poll 模型的创建
	if (ret < 0)
	{
		return SW_ERR;
	}
	main_reactor.ptr = serv;
	main_reactor.id = 0;
	/******************设置对应的回调函数 start*****************************/
	main_reactor.setHandle(&main_reactor, SW_EVENT_CLOSE, swServer_onClose);
	main_reactor.setHandle(&main_reactor, SW_EVENT_CONNECT, swServer_onAccept);
	main_reactor.setHandle(&main_reactor, SW_EVENT_TIMER, swServer_onTimer);
    /******************设置对应的回调函数 end*****************************/
	//把对应的fd添加到对应的 IO数中
	main_reactor.add(&main_reactor, serv->main_pipe.getFd(&serv->main_pipe, 0), SW_EVENT_CLOSE);
	if (serv->timer_interval != 0)
	{
		ret = swServer_timer_start(serv);
		if (ret < 0)
		{
			return SW_ERR;
		}
		main_reactor.add(&main_reactor, serv->timer_fd, SW_EVENT_TIMER);
	}

	SW_START_SLEEP;
	// socket bind listen  
	/********************在主线程之中 创建了listen fd start********************/
	ret = swServer_listen(serv, &main_reactor);

	/********************在主线程之中 创建了listen fd end********************/
	if (ret < 0)
	{
		return SW_ERR;
	}

	tmo.tv_sec = SW_MAINREACTOR_TIMEO;
	tmo.tv_usec = 0;

	//Signal Init
	swSignalInit();

	serv->onStart(serv);  //当服务启动的时候触发的回调函数
	//主线程进入事件循环
	main_reactor.wait(&main_reactor, &tmo);

	serv->onShutdown(serv); //关闭服务器是触发的对应的回调函数
	return SW_OK;
}

/**
 * 关闭连接
 */
int swServer_close(swServer *serv, swEvent *event)
{
	swEventClose cev;
	if(event->from_id > serv->poll_thread_num)
	{
		swWarn("Error: From_id > serv->poll_thread_num\n");
		return -1;
	}
	cev.fd = event->fd;
	cev.from_id = event->from_id;
	return serv->main_pipe.write(&serv->main_pipe, &cev, sizeof(cev));
}
/**
 * initializing server config, set default
 */
void swServer_init(swServer *serv)
{
	bzero(serv, sizeof(swServer));
	serv->backlog = SW_BACKLOG;
	serv->factory_mode = SW_MODE_CALL;
	serv->poll_thread_num = SW_THREAD_NUM;
	serv->daemonize = 0;

	serv->ringbuffer_size = SW_QUEUE_SIZE;

	serv->timeout_sec = SW_REACTOR_TIMEO_SEC;
	serv->timeout_usec = SW_REACTOR_TIMEO_USEC; //300ms;

	serv->timer_interval = 0;

	serv->writer_num = SW_CPU_NUM;
	serv->worker_num = SW_CPU_NUM;
	serv->max_conn = SW_MAX_FDS;
	serv->max_request = SW_MAX_REQUEST;

	serv->open_udp = 0;
	serv->open_cpu_affinity = 0;
	serv->open_tcp_nodelay = 0;

	serv->udp_max_tmp_pkg = SW_MAX_TMP_PKG;

	serv->timer_list = NULL;

	serv->onClose = NULL;
	serv->onConnect = NULL;
	serv->onStart = NULL;
	serv->onShutdown = NULL;
	serv->onReceive = NULL;
	serv->onTimer = NULL;
}
//服务器启动的时间
static int swServer_timer_start(swServer *serv)
{
	int timer_fd;
	struct itimerspec timer_set;
	struct timespec now;

	if (clock_gettime(CLOCK_REALTIME, &now) == -1)
	{
		swError("clock_gettime fail\n");
		return SW_ERR;
	}

	timer_fd = timerfd_create(CLOCK_REALTIME, TFD_NONBLOCK | TFD_CLOEXEC);
	if (timer_fd < 0)
	{
		swError("create timerfd fail\n");
		return SW_ERR;
	}

	timer_set.it_value.tv_sec = now.tv_sec;
	timer_set.it_value.tv_nsec = now.tv_nsec;
	timer_set.it_interval.tv_sec = serv->timer_interval;
	timer_set.it_interval.tv_nsec = 0;

	if (timerfd_settime(timer_fd, TFD_TIMER_ABSTIME, &timer_set, NULL) == -1)
	{
		swError("set timer fail\n");
		return SW_ERR;
	}
	serv->timer_fd = timer_fd;
	return SW_OK;
}
/************创建服务*************/
int swServer_create(swServer *serv)
{
	int ret = 0;
	//创建对应的管道 进行通讯
	ret = swPipeBase_create(&serv->main_pipe, 0);
	if (ret < 0)
	{
		swTrace("[swServerCreate]create event_fd fail\n");
		return SW_ERR;
	}
	//创始化线程池
	serv->poll_threads = sw_calloc(serv->poll_thread_num, sizeof(swThreadPoll));
	if (serv->poll_threads == NULL)
	{
		swError("[swServerCreate]calloc[0] fail\n");
		return SW_ERR;
	}
	//create factry object
	if (serv->factory_mode == SW_MODE_THREAD)
	{
		if (serv->writer_num < 1)
		{
			swError("serv->writer_num < 1\n");
			return SW_ERR;
		}
		ret = swFactoryThread_create(&(serv->factory), serv->writer_num);
	}  //process  模式创建进程
	else if (serv->factory_mode == SW_MODE_PROCESS)
	{    
		//读进程和写进程都不能为空
		if (serv->writer_num < 1 || serv->worker_num < 1)
		{
			swError("serv->writer_num < 1 or serv->worker_num < 1\n");
			return SW_ERR;
		} //最大请求书
		if (serv->max_request < 1)
		{
			swError("serv->max_request < 1 \n");
			return SW_ERR;
		}
		serv->factory.max_request = serv->max_request; // 最大请求数
		//创建进程服务
		ret = swFactoryProcess_create(&(serv->factory), serv->writer_num, serv->worker_num);
	}
	else//其他模式创建服务
	{   
		ret = swFactory_create(&(serv->factory));
	}
	if (ret < 0)
	{
		swError("[swServerCreate]create factory fail\n");
		return SW_ERR;
	}
	serv->factory.ptr = serv;
	serv->factory.onTask = serv->onReceive;
	if (serv->open_udp == 1)  //udp  服务器
	{
		serv->factory.onFinish = swServer_onFinish2;
	}
	else
	{   
		//tcp 服务器 完成时的回调函数
		serv->factory.onFinish = swServer_onFinish; 
	}
	return SW_OK;
}
//服务器关闭
int swServer_shutdown(swServer *serv)
{
	//stop all thread
	swoole_running = 0;
	return SW_OK;
}
//服务器释放对应的资源
int swServer_free(swServer *serv)
{
	if (serv->factory.shutdown != NULL)
	{
		serv->factory.shutdown(&(serv->factory));
	}
	if (serv->reactor.free != NULL)
	{
		serv->reactor.free(&(serv->reactor));
	}
	if (serv->poll_threads != NULL)
	{
		sw_free(serv->poll_threads);
	}
	if (serv->main_pipe.close != NULL)
	{
		serv->main_pipe.close(&serv->main_pipe);
	}
	if (serv->timer_interval != 0)
	{
		swServer_timer_free(serv);
	}
	return SW_OK;
}
//创建reactor 线程
static int swServer_poll_start(swServer *serv)
{  
	
	swThreadParam *param;
	swThreadPoll *poll_thread;
	int i;
	pthread_t pidt;

	for (i = 0; i < serv->poll_thread_num; i++)
	{
		poll_thread = &(serv->poll_threads[i]);
		param = sw_malloc(sizeof(swThreadParam));
		if (param == NULL)
		{
			swError("malloc fail\n");
			return SW_ERR;
		}
		if (serv->open_udp == 1)
		{
			poll_thread->udp_addrs = sw_calloc(serv->udp_max_tmp_pkg, sizeof(swUdpFd));
			if (poll_thread->udp_addrs == NULL)
			{
				swError("malloc fail\n");
				return SW_ERR;
			}
			poll_thread->c_udp_fd = 0;
		}
		param->object = serv;
		param->pti = i;
		//创建对应的reactor线程 并且进入事件循环
		pthread_create(&pidt, NULL, (void * (*)(void *)) swServer_poll_loop, (void *) param);
		poll_thread->ptid = pidt;
	}
	return SW_OK;
}
/**
 * only tcp
 */
int swServer_onFinish(swFactory *factory, swSendData *resp)
{   

	return swWrite(resp->fd, resp->data, resp->len);
}
/**
 * for udp + tcp
 */
int swServer_onFinish2(swFactory *factory, swSendData *resp)
{
	swServer *serv = factory->ptr;
	swThreadPoll *poll_thread = &(serv->poll_threads[resp->from_id]);
	int ret;
	swUdpFd *fd;
	//UDP
	if (resp->fd <= 0)
	{
		fd = &(poll_thread->udp_addrs[-resp->fd]);
		while (1)
		{
			ret = sendto(fd->sock, resp->data, resp->len, 0, (struct sockaddr *) &(fd->addr), sizeof(fd->addr));
			swTrace("sendto sock=%d|from_id=%d\n", fd->sock, resp->from_id);
			if (ret < 0)
			{
				if (errno == EINTR || errno == EAGAIN)
				{
					continue;
				}
				else
				{
					swTrace("sendto fail.errno=%d\n", errno);
				}
			}
			break;
		}
		return ret;
	}
	else
	{
		return swWrite(resp->fd, resp->data, resp->len);
	}
}
/**

 * Main Loop 主线程进入事件循环
 */
static int swServer_poll_loop(swThreadParam *param)
{
	swServer *serv = param->object;
	int ret, pti = param->pti;
	swReactor *reactor = &(serv->poll_threads[pti].reactor);
	struct timeval timeo;

	//cpu affinity setting   CPU绑定设置
	if(serv->open_cpu_affinity)
	{
		cpu_set_t cpu_set;
		CPU_ZERO(&cpu_set);
		CPU_SET(pti % SW_CPU_NUM, &cpu_set);
		if(0 != pthread_setaffinity_np(pthread_self(), sizeof(cpu_set), &cpu_set))
		{
			swTrace("pthread_setaffinity_np set fail\n");
		}
	}

	ret = swReactorEpoll_create(reactor, (serv->max_conn / serv->poll_thread_num) + 1);
	if (ret < 0)
	{
		return SW_ERR;
	}
	timeo.tv_sec = serv->timeout_sec;
	timeo.tv_usec = serv->timeout_usec; //300ms
	reactor->ptr = serv;
	reactor->id = pti;
	reactor->setHandle(reactor, SW_FD_CLOSE, swServer_poll_onClose);

	//Thread mode must copy the data.
	//will free after onFinish

	reactor->setHandle(reactor, SW_FD_TCP, swServer_poll_onReceive);
	reactor->setHandle(reactor, SW_FD_UDP, swServer_poll_onPackage);

	//main loop
	reactor->wait(reactor, &timeo);
	//shutdown
	reactor->free(reactor);
	sw_free(param);
	return SW_OK;
}
//接受客户端的数据发送
static int swServer_poll_onReceive(swReactor *reactor, swEvent *event)
{ 
	
	int ret, n;
	swServer *serv = reactor->ptr;
	swFactory *factory = &(serv->factory);
	swEventData buf;
	n = swRead(event->fd, buf.data, SW_BUFFER_SIZE);
	//swTrace("swRead data fd: %d\n", event->fd);
	if (n < 0)
	{
		swTrace("swRead error: %d\n", errno);
		return SW_ERR;
	}
	else if (n == 0)
	{    
		//数据读取完毕  需要服务器关闭
		swTrace("Close Event.FD=%d|From=%d\n", event->fd, event->from_id);
		return swServer_close(serv, event);
	}
	else
	{
		buf.fd = event->fd;
		buf.len = n;
		buf.from_id = event->from_id;
		//分发给对应的进程去处理
		ret = factory->dispatch(factory, &buf);
		swTrace("recv:%s|fd=%d\n",buf.data,event->fd);
		if(ret < 0)
		{
			swTrace("factory->dispatch fail\n");
			return SW_ERR;
		}
	}
	return SW_OK;
}

/**
 * for udp
 */
static int swServer_poll_onPackage(swReactor *reactor, swEvent *event)
{
	int ret;
	swServer *serv = reactor->ptr;
	swFactory *factory = &(serv->factory);
	swThreadPoll *poll_thread = &(serv->poll_threads[event->from_id]);
	swEventData buf;
	socklen_t addrlen = sizeof(poll_thread->udp_addrs[poll_thread->c_udp_fd].addr);

	bzero(buf.data, sizeof(buf.data));

	while (1)
	{
		ret = recvfrom(event->fd, buf.data, SW_BUFFER_SIZE, 0, &(poll_thread->udp_addrs[poll_thread->c_udp_fd].addr),
				&addrlen);
		if (ret < 0)
		{
			if (errno == EINTR)
			{
				continue;
			}
			return SW_ERR;
		}
		break;
	}
	poll_thread->udp_addrs[poll_thread->c_udp_fd].sock = event->fd;
	buf.fd = -poll_thread->c_udp_fd; //区分TCP和UDP
	buf.len = ret;
	buf.from_id = event->from_id;
	swTrace("recv package: %s|fd=%d|size=%d\n", buf.data, event->fd, ret);
	ret = factory->dispatch(factory, &buf);
	if(ret <0)
	{
		swTrace("factory->dispatch fail\n");
	}
	poll_thread->c_udp_fd++;
	if (poll_thread->c_udp_fd == serv->udp_max_tmp_pkg)
	{
		poll_thread->c_udp_fd = 0;
	}
	return SW_OK;
}

static int swServer_poll_onClose(swReactor *reactor, swEvent *event)
{
	swServer *serv = reactor->ptr;
	//swFactory *factory = &(serv->factory);
	return swServer_close(serv, event);
}

void swSignalInit(void)
{
	swSignalSet(SIGHUP, SIG_IGN, 1, 0);
	swSignalSet(SIGPIPE, SIG_IGN, 1, 0);
	swSignalSet(SIGUSR1, SIG_IGN, 1, 0);
	swSignalSet(SIGUSR2, SIG_IGN, 1, 0);
	swSignalSet(SIGTERM, swSignalHanlde, 1, 0);
}
//添加服务器的额外 监听进程事件
int swServer_addListen(swServer *serv, int type, char *host, int port)
{
	swListenList_node *listen_host = sw_malloc(sizeof(swListenList_node));
	listen_host->type = type;
	listen_host->port = port;
	listen_host->sock = 0;
	bzero(listen_host->host, SW_HOST_MAXSIZE);
	strncpy(listen_host->host, host, SW_HOST_MAXSIZE);
	LL_APPEND(serv->listen_list, listen_host);
	if (type == SW_SOCK_UDP || type == SW_SOCK_UDP6)
	{
		serv->open_udp = 1;
	}
	return SW_OK;
}
//listen  监听服务器
static int swServer_listen(swServer *serv, swReactor *reactor)
{  
	
	int sock;
	int reactor_i = 0;

	swListenList_node *listen_host;
	swReactor *poll_reactor;
     //listen_host->host = "127.0.0.1";
	listen_host->port = 9501;
	listen_host->type = 1;
	
	//LL_FOREACH(serv->listen_list, listen_host)
	//{  
		sock = swSocket_listen(listen_host->type, listen_host->host, listen_host->port, serv->backlog);
		if (sock < 0)
		{
			swTrace("Listen fail.type=%d|host=%s|port=%d|errno=%d\n",
					listen_host->type, listen_host->host, listen_host->port, errno);
			return SW_ERR;
		}
		//UDP
		if (listen_host->type == SW_SOCK_UDP || listen_host->type == SW_SOCK_UDP6)
		{
			poll_reactor = &(serv->poll_threads[reactor_i % serv->poll_thread_num].reactor);
			poll_reactor->add(poll_reactor, sock, SW_FD_UDP);
			reactor_i++;
		}
		//TCP
		else
		{   
			//注册连接事件回调函数  吧对应的listenfd  注册进去
			reactor->add(reactor, sock, SW_EVENT_CONNECT);
		}
		listen_host->sock = sock;
	//}
	return SW_OK;
}
//信号手柄设置  
void swSignalHanlde(int sig)
{
	switch (sig)
	{
	case SIGTERM:
		swoole_running = 0;
		break;
	default:
		break;
	}
	swSignalInit();
}
