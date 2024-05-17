#ifndef __COROUTINE_H__
#define __COROUTINE_H__

#include<stdio.h>
#include<malloc.h>
#include<ucontext.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<fcntl.h>

#define  CORSZ 1024
#define  STACKSZ 2014 * 64

//标记协成运行的状态
enum States
{
    DEAD,       //死亡
    READY,      //就绪
    RUNNING,    //运行
    SUSPEND     //阻塞
};

struct schedule;

//协成结构体
typedef struct coroutine
{
    //回调函数
    void * (*call_back)(struct schedule *s,void *argc);
    //回调函数参数
    void *argc;
    //协成上下文
    ucontext_t ctx;
    //协成栈
    char stack[STACKSZ];
    //协成状态
    enum States state; 
} coroutine_t;

//协成管理器
typedef struct schedule
{
     coroutine_t **coroutines;  //对应的协成结构体
     int current_id;            //当前协成id 如果没有运行的协成则设置为 -1
     ucontext_t ctx_main;        //主协成的上下文环境
     int max_id;               //最大下标
}schedule_t;

//初始化协成管理器
schedule_t * schedule_create();

//创建协成
int coroutine_create(schedule_t *s,void* (*call_back)(schedule_t*, void* argc),void* argc);

//启动协成
void coroutine_running(schedule_t *s,int id);

//让出cpu
void coroutine_yield(schedule_t *s);

//还原cpu
void coroutine_resume(schedule_t *s,int id);

//释放调度器
void schedule_destroy(schedule_t *s);

//判断协成是否运行完成
int schedule_finished(schedule_t* s);

#endif /*__COROUTINE_H__*/