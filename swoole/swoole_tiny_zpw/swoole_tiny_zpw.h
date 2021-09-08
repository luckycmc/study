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

/****************************打印对应的调试信息 end*****************************/
/*****************内存信息宏定义 start****************/
#define sw_malloc              malloc
#define sw_free(s)             free(s)
#define sw_calloc              calloc
#define sw_realloc             realloc
/*****************内存信息宏定义 end****************/

#endif