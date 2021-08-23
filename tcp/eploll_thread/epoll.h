#ifndef _EPOLL_SERVER_H
#define _EPOLL_SERVER_H

#include <stdio.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
 
#include <errno.h>


#define MAXLINE 10
#define OPEN_MAX 100
#define LISTENQ 20
#define SERV_PORT 8080  //监听的端口
#define INFTIM 1000


//线程池任务队列结构
struct task{
    
     int fd; //需要读写的文件描述符

     struct task *next; //下一个任务
}

//用于读写的两个方面的参数传递

struct user_data{

     int fd;
     unsigned int n_size;
     char line[MAXLINE];
}
//线程任务函数
void * readtask(void *args);
//void * writetask(void *args);
//设置非阻塞
void setnonblocking(int sock);
//服务器初始化
void init_server();

#endif