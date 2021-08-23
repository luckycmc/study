#ifndef MULTI_THREAD_H
#define MULTI_THREAD_H


#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>

#define RECVBUFSIZE 2048

//对应的处理函数
void *rec_func(void *arg);
//主函数
void master_thread();
#endif