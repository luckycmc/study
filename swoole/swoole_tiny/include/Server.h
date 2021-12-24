#ifndef SW_SERVER_H_
#define SW_SERVER_H_

#include "list.h"

#define SW_EVENT_CLOSE           5   //事件关闭
#define SW_EVENT_CONNECT         6   //事件连接
#define SW_EVENT_TIMER           7

#define SW_HOST_MAXSIZE          48
#define SW_MAX_TMP_PKG           1000
//udp fd
typedef struct _swUdpFd{
	struct sockaddr addr;
	int sock;
} swUdpFd;
//线程池
typedef struct _swThreadPoll   
{
	pthread_t ptid; //线程ID
	swReactor reactor;
	swUdpFd *udp_addrs;
	int c_udp_fd;
} swThreadPoll;
//监听者节点
typedef struct _swListenList_node
{
	struct _swListenList_node *next, *prev;  //双向链表节点
	int type;
	int port;
	int sock;
	char host[SW_HOST_MAXSIZE];
} swListenList_node;

typedef struct _swTimerList_node
{
	struct _swTimerList_node *next, *prev;
	int interval;
	int lasttime;
} swTimerList_node;

typedef struct swServer_s swServer;
// swoole server 结构体
struct swServer_s
{
	int backlog;
	int factory_mode;
	int poll_thread_num;
	int writer_num;
	int worker_num;
	int max_conn;
	int max_request;
	int timeout_sec;
	int timeout_usec;
	int daemonize;

	int timer_fd;
	int signal_fd;
	int event_fd;

	int timer_interval;
	int ringbuffer_size;

	int c_pti;           //schedule
	int udp_max_tmp_pkg; //UDP临时包数量，超过数量未处理将会被丢弃

	char open_udp;        //是否有UDP监听端口
	char open_cpu_affinity; //是否设置CPU亲和性
	char open_tcp_nodelay;  //是否关闭Nagle算法

	swPipe main_pipe;         //管道
	swReactor reactor;        // reactor  结构体
	swFactory factory;       
	swThreadPoll *poll_threads;   //线程池
	swListenList_node *listen_list;
	swTimerList_node *timer_list;

	void *ptr; //reserve
	void *ptr2; //reserve

	void (*onStart)(swServer *serv);  //服务启动
	int (*onReceive)(swFactory *factory, swEventData *data);//接受数据
	void (*onClose)(swServer *serv, int fd, int from_id);  //客户端关闭
	void (*onConnect)(swServer *serv, int fd, int from_id); //客户端连接
	void (*onShutdown)(swServer *serv);                    //服务器关闭停止时
	void (*onTimer)(swServer *serv, int interval);
};
int swServer_onFinish(swFactory *factory, swSendData *resp); //完成
int swServer_onFinish2(swFactory *factory, swSendData *resp);
int swServer_onClose(swReactor *reactor, swEvent *event);  //客户端关闭
int swServer_onAccept(swReactor *reactor, swEvent *event);  //接受客户端连接

void swServer_init(swServer *serv);   //服务器初始化
int swServer_start(swServer *serv);   //服务器启动
int swServer_addListen(swServer *serv, int type, char *host,int port); //添加监听者进程
int swServer_create(swServer *serv); // 创建服务
int swServer_free(swServer *serv);   // 释放服务
int swServer_close(swServer *factory, swEvent *event);  //服务器关闭
int swServer_shutdown(swServer *serv);                  //服务器 shutdown
int swServer_addTimer(swServer *serv, int interval);

#endif /* SW_SERVER_H_ */
