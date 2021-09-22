#include "./include/swoole_tiny_zpw.h"

struct sockitem { //
	int sockfd; // 
	int (*callback)(int fd, int events, void *arg);

	char recvbuffer[1024]; //
	char sendbuffer[1024]; //

};

// mainloop / eventloop --> epoll -->  
struct reactor {

	int epfd;
	struct epoll_event events[512];
};

int server_start();
int servser_create();
//创建reactor
int reactor_cteate();
//监听对应的listenfd 
int listenFd();
/**********对应的回调函数 start*************/
void onConnect();
void onReceive();
void onClose();
void onFinish();
/**********对应的回调函数 end*************/

/*************对应的客户端处理函数 start***************/
int recv_cb(int fd, int events, void *arg);
int send_cb(int fd, int events, void *arg);
int accept_cb(int fd, int events, void *arg);
//注册关闭的回调函数
int close_cb(int fd, int events, void *arg);
/*************对应的客户端处理函数 end***************/

/***************回调函数的实现 start**************/
void onConnect()
{
     swTrace("onConnect is comming\n");
}
void onFinish()
{
     swTrace("onFinish is comming\n");
}
void onReceive()
{
      swTrace("onReceive is comming\n");
}
void onClose()
{
     swTrace("onClose is comming\n");
}
/***************回调函数的实现 end**************/
//list fd 函数
int listenFd()
{
    int port = SW_PORT;

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		return -1;
	}

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(struct sockaddr_in));

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(sockfd, (struct sockaddr*)&addr, sizeof(struct sockaddr_in)) < 0) {
		return -2;
	}
    
	if (listen(sockfd, 5) < 0) {
		return -3;
	}
	//注册connect 连接回调函数的
    return sockfd;
}

struct reactor *eventloop = NULL;
//发送数据请求
int send_cb(int fd, int events, void *arg) 
{

	struct sockitem *si = (struct sockitem*)arg;

	send(fd, "hello world\n", 6, 0); //  发送给用户数据

	struct epoll_event ev;
	ev.events = EPOLLIN | EPOLLET;
	//ev.data.fd = clientfd;
	si->sockfd = fd;
	si->callback = recv_cb; //注册回调函数
	ev.data.ptr = si;

	epoll_ctl(eventloop->epfd, EPOLL_CTL_MOD, fd, &ev);
     //回调onFinish();函数
    onFinish();
}
//接受数据请求
int recv_cb(int fd, int events, void *arg) 
{

	//int clientfd = events[i].data.fd;
	struct sockitem *si = (struct sockitem*)arg;
	struct epoll_event ev;

	char buffer[1024] = {0};
	int ret = recv(fd, buffer, 1024, 0);
	if (ret < 0) {  //出现错误

		if (errno == EAGAIN || errno == EWOULDBLOCK) { //
			return -1;
		} else {
			
		}

		ev.events = EPOLLIN;
		//ev.data.fd = fd;
		epoll_ctl(eventloop->epfd, EPOLL_CTL_DEL, fd, &ev);

		close(fd);

		free(si);
	
	} else if (ret == 0) { //

		// 
		ev.events = EPOLLIN|EPOLLET;
		si->sockfd = fd;
		si->callback = close_cb;
		ev.data.ptr = si;
		//ev.data.fd = fd;
		epoll_ctl(eventloop->epfd, EPOLL_CTL_MOD, fd, &ev);  //EPOLL_CTL_DEL

		/*  close(fd);
		 free(si);
         onClose(); */
		
	} else {

		printf("Recv: %s, %d Bytes\n", buffer, ret);  //io操作写到屏幕页面上

		struct epoll_event ev;
		ev.events = EPOLLOUT | EPOLLET;
		//ev.data.fd = clientfd;
		si->sockfd = fd;
		si->callback = send_cb; //设置对应的回调函数
		ev.data.ptr = si;
		epoll_ctl(eventloop->epfd, EPOLL_CTL_MOD, fd, &ev);
       
	}

}

// 接受客户端连接
int accept_cb(int fd, int events, void *arg) 
{

	struct sockaddr_in client_addr;
	memset(&client_addr, 0, sizeof(struct sockaddr_in));
	socklen_t client_len = sizeof(client_addr);
	
	int clientfd = accept(fd, (struct sockaddr*)&client_addr, &client_len);
	if (clientfd <= 0) return -1;

	char str[INET_ADDRSTRLEN] = {0};
	printf("accept from %s at port %d\n", inet_ntop(AF_INET, &client_addr.sin_addr, str, sizeof(str)),
		ntohs(client_addr.sin_port));

	struct epoll_event ev;
	ev.events = EPOLLIN | EPOLLET; //出发模式
	//ev.data.fd = clientfd;

	struct sockitem *si = (struct sockitem*)malloc(sizeof(struct sockitem));
	si->sockfd = clientfd;
	si->callback = recv_cb;
	ev.data.ptr = si;
	
	epoll_ctl(eventloop->epfd, EPOLL_CTL_ADD, clientfd, &ev);
	//接受数据回调函数
	 onConnect();     //出发有链接进来的回调函数
	 return clientfd;
}
//关闭事件的处理函数
int close_cb(int fd, int events, void *arg)
{
     printf("closing connection %d\n", fd);
	 struct sockitem *si = (struct sockitem*)arg;
	 struct epoll_event ev;
	 ev.events = EPOLLIN;
	 //需要删除对应的fd 节点
	 epoll_ctl(eventloop->epfd,EPOLL_CTL_DEL, fd, &ev);
	 close(fd);
	 free(si);
	 //回调对应的回调函数
	 onClose();
}

//创建对应的reactor 模型
int reactor_cteate()
{
    eventloop = (struct reactor*)malloc(sizeof(struct reactor));
	// epoll opera

	eventloop->epfd = epoll_create(1); // 红黑树节点的创建
	printf("eventloop->epfd is %d\n", eventloop->epfd);
	if (eventloop->epfd < -1)
	{
		return -1;
	}
	return eventloop->epfd;
	
}
//主函数
int main(int argc, char *argv[]) 
{
    int epfd ;
	int sockfd = listenFd();   // listen fd

	printf("server is starting\n");

    epfd = reactor_cteate();  //epoll_create 的创建

	struct epoll_event ev;
	ev.events = EPOLLIN;
	//ev.data.fd = sockfd; //int idx = 2000;
	
	struct sockitem *si = (struct sockitem*)malloc(sizeof(struct sockitem));
	si->sockfd = sockfd;
	si->callback = accept_cb;   //注册对应的回调函数
	ev.data.ptr = si;
	
	epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);

	while (1)  // 进入事件循环
     {
		int nready = epoll_wait(eventloop->epfd, eventloop->events, 512, -1);

		if (nready < -1) {
			break;
		}else if (nready == 0)
          {
              continue;  //进入下一轮事件循环
          }
          
		int i = 0;
          //处理准备就绪的io的IO
		for (i = 0;i < nready;i ++) 
          {

			if (eventloop->events[i].events & EPOLLIN) {
				//printf("sockitem\n");
				struct sockitem *si = (struct sockitem*)eventloop->events[i].data.ptr; //检测数用户设置的参数
				//触发回调函数
				si->callback(si->sockfd, eventloop->events[i].events, si);
			}

			if (eventloop->events[i].events & EPOLLOUT) {

				struct sockitem *si = (struct sockitem*)eventloop->events[i].data.ptr;//检测数用户设置的参数
				//触发回调函数
				si->callback(si->sockfd, eventloop->events[i].events, si);
			}
		}
	}
    return 0;
	
}