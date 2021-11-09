/* A simple event-driven programming library. Originally I wrote this code
 * for the Jim's event-loop (Jim is a Tcl interpreter) but later translated
 * it in form of a library for easy reuse.
 *
 * Copyright (c) 2006-2012, Salvatore Sanfilippo <antirez at gmail dot com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of Redis nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __AE_H__
#define __AE_H__

#include <time.h>

#define AE_OK 0
#define AE_ERR -1

#define AE_NONE 0
/**
   当套接字变得可读时（客户端对套接字执行 write 操作，或者执行 close 操作）， 或者有新的可应答（acceptable）
   套接字出现时（客户端对服务器的监听套接字执行 connect 操作）， 套接字产生 AE_READABLE 事件。
 * 
 */
#define AE_READABLE 1    //设置读事件
/**

 * 当套接字变得可写时（客户端对套接字执行 read 操作）， 套接字产生 AE_WRITABLE 事件
 * 
 */
#define AE_WRITABLE 2    //设置写事件

#define AE_FILE_EVENTS 1
#define AE_TIME_EVENTS 2
//如果一个套接字又可读又可写的话,那么服务器将先读套接字， 后写套接字
#define AE_ALL_EVENTS (AE_FILE_EVENTS|AE_TIME_EVENTS)   //读写事件同时监控 
#define AE_DONT_WAIT 4

#define AE_NOMORE -1
#define AE_DELETED_EVENT_ID -1

/* Macros */
#define AE_NOTUSED(V) ((void) V)

struct aeEventLoop;

/* Types and data structures */
typedef void aeFileProc(struct aeEventLoop *eventLoop, int fd, void *clientData, int mask);
typedef int aeTimeProc(struct aeEventLoop *eventLoop, long long id, void *clientData);
typedef void aeEventFinalizerProc(struct aeEventLoop *eventLoop, void *clientData);
typedef void aeBeforeSleepProc(struct aeEventLoop *eventLoop);

// Redis中的事件驱动库只关注网络IO，以及定时器。该事件库处理下面两类事件：
/* File event structure */
// 文件事件(file  event)：用于处理 Redis 服务器和客户端之间的网络IO
typedef struct aeFileEvent {   //文件结构体
    int mask; /* one of AE_(READABLE|WRITABLE) */    //需要监控的读写标志位
    aeFileProc *rfileProc;      //读写事件处理函数
    aeFileProc *wfileProc;
    void *clientData;          //事件参数指针
} aeFileEvent;
//时间事件(time  eveat)：Redis 服务器中的一些操作（比如serverCron函数）
// 需要在给定的时间点执行，而时间事件就是处理这类定时操作的。
/* Time event structure */   //定时器
typedef struct aeTimeEvent {
    long long id; /* time event identifier. */   /*定时器事件的ID*/
    long when_sec; /* seconds */                 /*触发的秒*/
    long when_ms; /* milliseconds */           /*触发的毫秒*/  
    aeTimeProc *timeProc;
    aeEventFinalizerProc *finalizerProc; /*// 定时事件清理函数，当删除定时事件的时候会被调用*/
    void *clientData;
    struct aeTimeEvent *next;  /*下一个定时器对象，整体采用链表维护，作者好像知道链表维护的效率不是很高，推荐使用跳表，为啥不推荐使用红黑树嘞。*/
} aeTimeEvent;

/* A fired event */
typedef struct aeFiredEvent {   //就绪事件
    int fd;
    int mask;
} aeFiredEvent;

/* State of an event based program */
typedef struct aeEventLoop {
    int maxfd;   /* highest file descriptor currently registered */ /*事件管理器能管理的文件描述符的最大值*/
    int setsize; /* max number of file descriptors tracked */
    long long timeEventNextId;
    time_t lastTime;     /* Used to detect system clock skew */
    aeFileEvent *events; /* Registered events */
    aeFiredEvent *fired; /* Fired events */ /*触发事件数组，这里面放的是经过系统判断，有对应事件发生的文件描述符的存放数组，和上面一样，都采用数组维护*/
    aeTimeEvent *timeEventHead;   //时间事件
    int stop;
    void *apidata; /* This is used for polling API specific data */
    aeBeforeSleepProc *beforesleep;
} aeEventLoop;

/* Prototypes */
aeEventLoop *aeCreateEventLoop(int setsize);
void aeDeleteEventLoop(aeEventLoop *eventLoop);
void aeStop(aeEventLoop *eventLoop);
int aeCreateFileEvent(aeEventLoop *eventLoop, int fd, int mask,
        aeFileProc *proc, void *clientData);
void aeDeleteFileEvent(aeEventLoop *eventLoop, int fd, int mask);
int aeGetFileEvents(aeEventLoop *eventLoop, int fd);
long long aeCreateTimeEvent(aeEventLoop *eventLoop, long long milliseconds,
        aeTimeProc *proc, void *clientData,
        aeEventFinalizerProc *finalizerProc);
int aeDeleteTimeEvent(aeEventLoop *eventLoop, long long id);
int aeProcessEvents(aeEventLoop *eventLoop, int flags);
int aeWait(int fd, int mask, long long milliseconds);
void aeMain(aeEventLoop *eventLoop);
char *aeGetApiName(void);
void aeSetBeforeSleepProc(aeEventLoop *eventLoop, aeBeforeSleepProc *beforesleep);
int aeGetSetSize(aeEventLoop *eventLoop);
int aeResizeSetSize(aeEventLoop *eventLoop, int setsize);

#endif
