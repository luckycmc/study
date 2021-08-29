#include "swoole_tiny_zpw.h"

typedef struct swReactorEpoll_s swReactorEpoll;
//fd 对应的事件类型
typedef struct _swFd
{
   
     uint32_t fd;      //文件描述符
     uint32_t fdtype;  // 对应的事件

}swFd;
// reactor 句柄
struct swReactorEpoll_s
{
     int epfd;// 红黑树根节点
     int event_max ;  //事件最大个数
     struct epoll_event *events ;   //对应的事件指针
}

/*******************reactor 对应的操作事件 start**************************/
int swReactorEpoll_add(swReactor *reactor,int fd,int fdtype);
int swReactorEpoll_del(swReactor *reactor,int fd);
int swReactorEpoll_wait(swReactor *reactor, struct timeval *timeo);
int swReactorEpoll_del(swReactor *reactor);
/*******************reactor 对应的操作事件 end**************************/

//reactor 的创建
int swReactorEpoll_create(swReactor *reactor, int max_event_num)
{
     //创建reactor
     swReactorEpoll *reactor_object = sw_malloc(sizeof(swReactorEpoll));
     if(swReactorEpoll == NULL){
          
           swTrace("[swReactorEpollCreate] malloc[0] fail\n");
		 return -1;
     }
     reactor->object = reactor_object;
     reactor_object->events = sw_calloc(max_event_num,sizeof(struct epoll_event));
     if(reactor_object->events == NULL){
         
            swTrace("[swReactorEpollCreate] malloc[0] fail\n");
		   return -1;
     }
     //创建epoll
     reactor_object->event_max = 0;
     reactor_object->epfd = epoll_create(512);
     //创建失败
     if (reactor_object->epfd < 0)
	{
		swTrace("[swReactorEpollCreate] epoll_create[0] fail\n");
		return SW_ERR;
	}
     //绑定对应的函数

}// func end