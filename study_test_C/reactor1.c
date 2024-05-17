#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <pthread.h>

#include <errno.h>
#include <sys/epoll.h>

#define BUFFER_SIZE 1024

#define CLIENT_MAX_SIZE 1024
//socket item 对应的节点
struct socketitem
{
      int socketfd;
      //回调函数
      int (*callback)(int fd, int event, void *arg);

      char readbuf[BUFFER_SIZE]];
      char writebuf[BUFFER_SIZE];
};

//main_loop 事件

struct reactor {
    
      int epfd;
      struct epoll_event events[CLIENT_MAX_SIZE];
};
//全局实例化 eventloop 
struct reactor *eventloop = NULL;
//接受客户端连接
int accept_cb(int fd, int events, void *arg) {

}
//主函数
int main(int argc, char *argv)
{
     if (argc < 2) {
		return -1;
	}

	int port = atoi(argv[1]);  //获取对应的端口号
    //创建socket
    int socktfd = socket(AF_INET, SOCK_STREAM,0);

    if(socktfd < 0) {
        return -1;
    }

    //实例化监听的地址
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    //bind 地址
    if (bind(socktfd,(struct sockaddr *)&addr,sizeof(struct sockaddr_in))<0)
    {
          return -2;
    }
    //监听socket
    if (listen(socktfd,5)<-1)
    {
        return -3;
    }
    
    eventloop = (struct reactor *)malloc(sizeof(struct reactor));//指定空间

    //创建epoll 红黑树跟节点
    eventloop->epfd = epoll_create(1);
    
     //创建event 事件
    
    struct epoll_event ev; //创建event事件
    ev.events = EPOLLIN;   //可读事件

    struct socketitem *si = (struct socketitem *)malloc(sizeof(struct socketitem));
    si->socketfd = socketfd;
    si->callback = accept_cb; //指向回调函数
    
    ev.data.ptr = si;

    //加入到epoll
    int result = epoll_ctl(eventloop->epfd,EPOLL_CTL_ADD,socktfd,&ev);
    if (result < 0)
    {
        return -1;
    }
    
    //阻塞 等待 客户端连接
    while(1)
    {
         //等待内核返回的处理好的IO
         int nready = epoll_wait(eventloop->epfd,eventloop->events,1024,-1);
         if (nready < -1)
         {
             continue; //进入下一轮循环
         }
        
         //查找已经准备完毕的fd 的读写事件
         int i =0;
         for (i = 0; i < nready; i++)
         {
               //数据可读
               if (eventloop->event[i].event & EPOLLIN)
               {
                     //获取对应当前的结构数据
                     struct socketitem *si = (struct socketitem *)eventloop->event[i].data.ptr;
                     //出发回调函数
                     si->callback(si->socketfd,eventloop->events[i].events,si);
               }
               
               //数据可写
               if (eventloop->event[i].event & EPOLLOUT)
               {
                     //获取对应当前的结构数据
                     struct socketitem *si = (struct socketitem *)eventloop->event[i].data.ptr;
                     //出发回调函数
                     si->callback(si->socketfd,eventloop->events[i].events,si);

               }
               
         }
         
    }
}


