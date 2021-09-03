#include "./include/swoole.h"
#include <string.h>

typedef struct swReactorEpoll_s swReactorEpoll;
typedef struct _swFd
{
	uint32_t fd;
	uint32_t fdtype;
} swFd;

int swReactorEpoll_add(swReactor *reactor, int fd, int fdtype);
int swReactorEpoll_del(swReactor *reactor, int fd);
int swReactorEpoll_wait(swReactor *reactor, struct timeval *timeo);
void swReactorEpoll_free(swReactor *reactor);

struct swReactorEpoll_s
{
	int epfd;
	int event_max;
	struct epoll_event *events;
};
/**
 * 相等于PHP 中的构造方法 初始化话属性和对应的方法 
  reactor  的创建
*/
int swReactorEpoll_create(swReactor *reactor, int max_event_num)
{  
	
	//create reactor object
	swReactorEpoll *reactor_object = sw_malloc(sizeof(swReactorEpoll));
	if (reactor_object == NULL)
	{
		swTrace("[swReactorEpollCreate] malloc[0] fail\n");
		return SW_ERR;
	}
	reactor->object = reactor_object;
	reactor_object->events = sw_calloc(max_event_num, sizeof(struct epoll_event));

	if (reactor_object->events == NULL)
	{
		swTrace("[swReactorEpollCreate] malloc[1] fail\n");
		return SW_ERR;
	}
	//epoll create
	reactor_object->event_max = 0;
	reactor_object->epfd = epoll_create(512);
	if (reactor_object->epfd < 0)
	{
		swTrace("[swReactorEpollCreate] epoll_create[0] fail\n");
		return SW_ERR;
	}
	//binding method
	/*绑定对应的操作事件 添加 删除 等待返回 */
	reactor->add = swReactorEpoll_add;
	reactor->del = swReactorEpoll_del;
	reactor->wait = swReactorEpoll_wait;
	reactor->free = swReactorEpoll_free;
	reactor->setHandle = swReactor_setHandle; // 对应的处理函数 也就是回调函数
	return SW_OK;
}
//释放reactor
void swReactorEpoll_free(swReactor *reactor)
{
	swReactorEpoll *this = reactor->object;
	close(this->epfd);  //关闭 跟节点
	sw_free(this->events); // 释放内存
	sw_free(this);     //释放内存
}
//reactor 添加对应的fd
int swReactorEpoll_add(swReactor *reactor, int fd, int fdtype)
{  
	
	swReactorEpoll *this = reactor->object;
	struct epoll_event e; //实例化事件结构体
	swFd fd_;
	int ret;
	bzero(&e, sizeof(struct epoll_event));

	fd_.fd = fd;
	fd_.fdtype = fdtype;
	//e.data.u64 = 0;
	//e.events = EPOLLIN | EPOLLOUT;
	e.events = EPOLLIN | EPOLLET; //边缘出发 写事件
	// e.data 主要用于保存 应用程序的关联数据  怎么设置的到时候怎么取出来 绑定对应的回调函数
	//  C 库函数 void *memcpy(void *str1, const void *str2, size_t n) 从存储区 str2 复制 n 个字节到存储区 str1
	memcpy(&(e.data.u64), &fd_, sizeof(fd_));

	swTrace("[THREAD #%ld]EP=%d|FD=%d\n", pthread_self(), this->epfd, fd);
	//把当前的fd添加到对应的 红黑树种 什么操作 那个io 处理也就是fd  &e 绑定对应的关联事件
	ret = epoll_ctl(this->epfd, EPOLL_CTL_ADD, fd, &e);
	if (ret < 0)
	{
		swTrace("[THREAD #%ld]add event fail.Ep=%d|fd=%d\n", pthread_self(), this->epfd, fd);
		return SW_ERR;
	}
	this->event_max++;
	return SW_OK;
}
//清除掉对应的当前fd
int swReactorEpoll_del(swReactor *reactor, int fd)
{
	swReactorEpoll *this = reactor->object;
	struct epoll_event e;
	int ret;
	e.data.fd = fd;   // 只设置了fd 没有设置其他额外的参数
	//e.data.u64 = 0;
	//e.events = EPOLLIN | EPOLLOUT;
	e.events = EPOLLIN | EPOLLET;
	//删除掉对应的节点
	ret = epoll_ctl(this->epfd, EPOLL_CTL_DEL, fd, &e);
	if (ret < 0)
	{
		return -1;
	}
	this->event_max--;
	return SW_OK;
}
//epoll_wait等待就绪的IO  IO处理器   等待各种IO事件的到来
int swReactorEpoll_wait(swReactor *reactor, struct timeval *timeo)
{
	swEvent ev;
	swFd fd_;
	swReactorEpoll *this = reactor->object;
	int i, nready, ret;

	while (swoole_running > 0)
	{  
	
		nready = epoll_wait(this->epfd, this->events, this->event_max + 1, timeo->tv_sec * 1000 + timeo->tv_usec / 1000);
         //错误处理
		if (nready < 0)
		{
			//swTrace("epoll error.EP=%d | Errno=%d\n", this->epfd, errno);
			if(swReactor_error(reactor) < 0)
			{
				return SW_ERR;
			}
			else
			{
				continue;
			}
		}
		else if (nready == 0)  //没有就绪的io
		{
			continue;
		}
		for (i = 0; i < nready; i++)  //处理对应就绪IO的个数
		{  
			
			//printf("event coming.Ep=%d|fd=%d\n", this->epfd, this->events[i].data.fd);
			if (this->events[i].events & EPOLLIN) //可写事件
			{
				swTrace("event coming.Ep=%d|fd=%d\n", this->epfd, this->events[i].data.fd);

				memcpy(&fd_, &(this->events[i].data.u64), sizeof(fd_));
				ev.fd = fd_.fd;
				ev.from_id = reactor->id;
				ev.type = fd_.fdtype;
				//触发对应的回调函数  事件分发器 对应的事件由对应的回调函数处理handle()回调函数处理
				reactor->handle[ev.type](reactor, &ev); //通过对应的ev.type 获取对应的回调函数 然后执行对应的
				//的回调函数
				swTrace("[THREAD #%ld]event finish.Ep=%d|ret=%d\n", pthread_self(), this->epfd, ret);
			}
			//epoll out 事件
			if (this->events[i].events & EPOLLOUT)// 可读事件
			{   
				
				swTrace("event coming.Ep=%d|fd=%d\n", this->epfd, this->events[i].data.fd);

				memcpy(&fd_, &(this->events[i].data.u64), sizeof(fd_));
				ev.fd = fd_.fd;
				ev.from_id = reactor->id;
				ev.type = fd_.fdtype;
				  //触发对应的回调函数
				reactor->handle[ev.type](reactor, &ev);
                swTrace("[THREAD #%ld]event epoll_out.Ep=%d|ret=%d\n", pthread_self(), this->epfd, ret);
			}
		}
	}
	return 0;
}
