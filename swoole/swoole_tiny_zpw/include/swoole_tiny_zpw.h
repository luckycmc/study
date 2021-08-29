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

/****************************打印对应的调试信息 end*****************************/
/*****************内存信息宏定义 start****************/
#define sw_malloc              malloc
#define sw_free(s)             free(s)
#define sw_calloc              calloc
#define sw_realloc             realloc
/*****************内存信息宏定义 end****************/

/****************基本变量 start********/
#define   SW_BUFFER_SIZE    1024

typedef unsigned int uint32_t;

/****************基本变量 end********/
//事件结构体
typedef struct _swEvevtData
{
      int fd; //fd
      int len; //长度
      int from_id; // reactor_id //哪一个reactor 线程
      char len[SW_BUFFER_SIZE];

}swEvevtData;
//发送数据结构体
typedef struct _swSendData
{
	int   fd;
	int   len;
	int   from_id;  // reactor_id //哪一个reactor 线程
	char *data;
} swSendData;
//事件结构体
typedef struct _swEvent
{
	int from_id; //Reactor Id
	int fd;
	int type;
} swEvent;     // fd 对应的类型
//关闭实践结构体
typedef struct _swEventClose
{
	int from_id; //Reactor Id
	int fd;
} swEventClose;
//连接事件
typedef struct _swEventConnect
{
	int from_id;     //Reactor Id
	int conn_fd;     // 连接fd
	int serv_fd;     // serv_fd
	struct sockaddr_in addr;  //
	socklen_t addrlen;
} swEventConnect;

/************回调函数 start*************/
typedef int (*swHandle)(swEventData *buf);
int swReactorEpoll_create(swReactor *reactor, int max_event_num);
typedef int (*swReactor_handle)(swReactor *reactor, swEvent *event);
/************回调函数 end*************/
typedef struct swReactor_s swReactor;
//reactor  结构体
struct swReactor_s
{
	void *object;   //首先 object 是具体 Reactor 对象的首地址，
                       //ptr 是拥有 Reactor 对象的类的指针，
	void *ptr;        //reserve
	int  id;           //Reactor ID
	int  running;    //该reactor 是否在运行

	swReactor_handle handle[SW_MAX_FDTYPE];  //回调函数数组
	//swFactory *factory;
	int (*add)(swReactor *, int, int);    //添加
	int (*del)(swReactor *, int);         // 删除
	int (*wait)(swReactor *, struct timeval *); // 等待
	void (*free)(swReactor *);  //释放
	int (*setHandle)(swReactor *, int, swReactor_handle); //设置手柄
};

#endif