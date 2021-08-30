#ifndef SWOOLE_H_ZPW_
#define SWOOLE_H_ZPW_

/***********************加载公用的头文件 start ********************************/
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <sched.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <pthread.h>
#include <netinet/tcp.h>
/***********************加载公用的头文件 end ********************************/

#define SW_DEBUG               //启用调试模式，请注释掉此行代码 调试软件的网站的代码

//返回当前的机器的CPU
#define SW_CPU_NUM             sysconf(_SC_NPROCESSORS_ONLN)

/****************************打印对应的调试信息 strat*****************************/
#ifdef SW_DEBUG
#define swTrace(str,...)       {printf("[%s:%d:%s]"str,__FILE__,__LINE__,__func__,##__VA_ARGS__);}
#else
#define swTrace(str,...)
#endif

#ifdef SW_DEBUG
#define swError(str,...)       {printf("[%s:%d:%s]"str,__FILE__,__LINE__,__func__,##__VA_ARGS__);exit(1);}
#else
#define swError(str,...)       {snprintf(sw_error,SW_ERROR_MSG_SIZE,"[%s:%d:%s]"str,__FILE__,__LINE__,__func__,##__VA_ARGS__);}
#endif

#ifdef SW_DEBUG
#define swWarn(str,...)       {printf("[%s:%d:%s]"str,__FILE__,__LINE__,__func__,##__VA_ARGS__);}
#else
#define swWarn(str,...)       {snprintf(sw_error,SW_ERROR_MSG_SIZE,"[%s:%d:%s]"str,__FILE__,__LINE__,__func__,##__VA_ARGS__);}
#endif
#define SW_MAX_FDTYPE          32  //32 kinds of event 32个事件
#define SW_PORT                8080  //对应的端口
#define SW_BUFFER_SIZE         1024
/****************************打印对应的调试信息 end*****************************/
/*****************内存信息宏定义 start****************/
#define sw_malloc              malloc
#define sw_free(s)             free(s)
#define sw_calloc              calloc
#define sw_realloc             realloc
/*****************内存信息宏定义 end****************/

/****************基本变量 start********/

typedef unsigned int uint32_t;

/****************基本变量 end********/

/*******************对应的事件start *********************/
#define SW_EVENT_CLOSE           5   //事件关闭
#define SW_EVENT_CONNECT         6   //事件连接
#define SW_EVENT_TIMER           7
/*******************对应的事件end *********************/
//事件数据结构体
typedef struct _swEventData
{
	int fd;
	int len;
	int from_id; //Reactor Id
	char data[SW_BUFFER_SIZE];

} swEventData;

//发送数据结构体
typedef struct _swSendData
{
	int fd;
	int len;
	int from_id;
	char *data;

} swSendData; 
//事件结构体
typedef struct _swEvent
{
	int from_id; //Reactor Id
	int fd;
	int type;
} swEvent;
//关闭实践结构体
typedef struct _swEventClose
{
	int from_id; //Reactor Id
	int fd;
} swEventClose;
//连接事件截水沟头
typedef struct _swEventConnect
{
	int from_id; //Reactor Id
	int conn_fd;
	int serv_fd;
	struct sockaddr_in addr;
	socklen_t addrlen;
} swEventConnect;


typedef int (*swHandle)(swEventData *buf);
typedef void (*swSignalFunc)(int);
typedef void (*swCallback)(void *); //回调函数
typedef struct swReactor_s swReactor;
typedef int (*swReactor_handle)(swReactor *reactor, swEvent *event);

// 管道 
typedef struct _swPipe
{
	void *object;
	int blocking;
	int (*read)(struct _swPipe *, void *recv, int length);
	int (*write)(struct _swPipe *, void *send, int length);
	int (*getFd)(struct _swPipe *, int isWriteFd);
	void (*close)(struct _swPipe *);
} swPipe;
/*********************进程结构体 start *************************************/
typedef struct _swFactory 
{
	void *object;
	int id; //Factory ID
	int running;   //是否运行
	int max_request; //worker进程最大请求数量
	void *ptr; //server object
	swReactor *reactor; //reserve for reactor

	int (*start)(struct _swFactory *);   //进程启动
	int (*dispatch)(struct _swFactory *, swEventData *); //进程分发
	int (*finish)(struct _swFactory *, swSendData *);   //完成

} swFactory;
/*********************进程结构体end *************************************/

//reactor  结构体
struct swReactor_s
{
	void *object;
	void *ptr; //reserve
	int id; //Reactor ID
	int running;

	swReactor_handle handle[SW_MAX_FDTYPE];
	swFactory *factory;

	int (*add)(swReactor *, int, int);    //添加
	int (*del)(swReactor *, int);         // 删除
	int (*wait)(swReactor *, struct timeval *); // 等待
	void (*free)(swReactor *);  //释放
	int (*setHandle)(swReactor *, int, swReactor_handle); //设置手柄
};


/***********************创建服务结构体 start********************************/ 
typedef struct swServer_s swServer;

struct swServer_s
{
    int backlog;
	int factory_mode;
	int worker_num;
	int max_conn;
	int max_request;
	int daemonize;
	int event_fd;
	char open_cpu_affinity; //是否设置CPU亲和性

	swPipe main_pipe;         //管道       用于通讯
	swReactor reactor;        // reactor  结构体

	void *ptr; //reserve
	void *ptr2; //reserve

	void (*onStart)(swServer *serv);  //服务启动
	int (*onReceive)(swFactory *factory, swEventData *data);//接受数据
	void (*onClose)(swServer *serv, int fd, int from_id);  //客户端关闭
	void (*onConnect)(swServer *serv, int fd, int from_id); //客户端连接
};

//创建对应的函数
int swServer_onClose(swReactor *reactor, swEvent *event);  //客户端关闭
int swServer_onAccept(swReactor *reactor, swEvent *event);  //接受客户端连接

void swServer_init(swServer *serv);   //服务器初始化
int swServer_start(swServer *serv);   //服务器启动
int swServer_create(swServer *serv); // 创建服务
int swServer_free(swServer *serv);   // 释放服务
int swServer_close(swServer *factory, swEvent *event);   //服务关闭

/***********************创建服务结构体 end********************************/ 


#endif