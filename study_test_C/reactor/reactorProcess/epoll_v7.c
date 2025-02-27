

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <pthread.h>

#include <errno.h>
#include <sys/epoll.h>

#define BUFFER_LENGTH		1024

//保存一个IO 状态
struct sockitem { //
	int sockfd;  //对应的socket fd

	int (*callback)(int fd, int events, void *arg); //反应堆对应的回调函数

	char recvbuffer[BUFFER_LENGTH]; //
	char sendbuffer[BUFFER_LENGTH];

	int rlength;
	int slength;
};

// 保存所有IO的全局变量
// mainloop / eventloop --> epoll -->  
struct reactor {

	int epfd;
	struct epoll_event events[512];

};

//全局 eventloop
struct reactor *eventloop = NULL;


int recv_cb(int fd, int events, void *arg);


int send_cb(int fd, int events, void *arg) {

	struct sockitem *si = (struct sockitem*)arg;

	send(fd, si->sendbuffer, si->slength, 0); //

	struct epoll_event ev;  //对应的事件
	ev.events = EPOLLIN | EPOLLET;
	//ev.data.fd = clientfd;
	si->sockfd = fd;
	si->callback = recv_cb;
	ev.data.ptr = si;

	epoll_ctl(eventloop->epfd, EPOLL_CTL_MOD, fd, &ev);

}

//  ./epoll 8080

int recv_cb(int fd, int events, void *arg) {

	//int clientfd = events[i].data.fd;
	struct sockitem *si = (struct sockitem*)arg;
	struct epoll_event ev;

	//char buffer[1024] = {0};
	int ret = recv(fd, si->recvbuffer, BUFFER_LENGTH, 0);
	if (ret < 0) {

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
		printf("disconnect %d\n", fd);

		ev.events = EPOLLIN;
		//ev.data.fd = fd;
		epoll_ctl(eventloop->epfd, EPOLL_CTL_DEL, fd, &ev);

		close(fd);

		free(si);
		
	} else {

		printf("Recv: %s, %d Bytes\n", si->recvbuffer, ret);
        
		//si->recvbuffer
		/**
		  当成协议解析
		  http 
		  websocket
		*/
		si->rlength = ret;
		memcpy(si->sendbuffer, si->recvbuffer, si->rlength);
		si->slength = si->rlength;

		struct epoll_event ev;
		ev.events = EPOLLOUT | EPOLLET;
		//ev.data.fd = clientfd;
		si->sockfd = fd;
		si->callback = send_cb;
		ev.data.ptr = si;

		epoll_ctl(eventloop->epfd, EPOLL_CTL_MOD, fd, &ev);

	}

}


int accept_cb(int fd, int events, void *arg) {

	struct sockaddr_in client_addr;
	memset(&client_addr, 0, sizeof(struct sockaddr_in));
	socklen_t client_len = sizeof(client_addr);
	
	int clientfd = accept(fd, (struct sockaddr*)&client_addr, &client_len);
	if (clientfd <= 0) return -1;

	char str[INET_ADDRSTRLEN] = {0};
	printf("recv from %s at port %d\n", inet_ntop(AF_INET, &client_addr.sin_addr, str, sizeof(str)),
		ntohs(client_addr.sin_port));

	struct epoll_event ev;
	ev.events = EPOLLIN | EPOLLET;
	//ev.data.fd = clientfd;

	struct sockitem *si = (struct sockitem*)malloc(sizeof(struct sockitem));
	si->sockfd = clientfd;
	si->callback = recv_cb;
	ev.data.ptr = si;   
	
	epoll_ctl(eventloop->epfd, EPOLL_CTL_ADD, clientfd, &ev);
	
	return clientfd;
}
/**
  typedef union epoll_data {
    void* ptr;             //空指针联合体用户可以自定义处理
    int     fd;            //对应放入fd
    uint32_t  u32;
    uint64_t  u64;
  } epoll_data_t;
  
  struct epoll_event {
    uint32_t  events;    // epoll events
   epoll_data_t  data;  // user data variable
 };
**/
int main(int argc, char *argv[]) {

	if (argc < 2) {
		return -1;
	}

	int port = atoi(argv[1]);

	

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

	eventloop = (struct reactor*)malloc(sizeof(struct reactor));
	// epoll opera

	eventloop->epfd = epoll_create(1);
	
	struct epoll_event ev; // 对应的事件初始化 为当前的fd设置对应的事件操作系统内置的结构体
	ev.events = EPOLLIN;
	
	struct sockitem *si = (struct sockitem*)malloc(sizeof(struct sockitem));
	si->sockfd = sockfd;
	si->callback = accept_cb; // 设置对应的回调函数
	ev.data.ptr = si; //存放用户自定义的功能处理 ptr是一个 void * 指针
	//对应的socket 加入到epoll 中去
	epoll_ctl(eventloop->epfd, EPOLL_CTL_ADD, sockfd, &ev);

	while (1) {
        //进入到事件循环
		int nready = epoll_wait(eventloop->epfd, eventloop->events, 512, -1);
		if (nready < -1) {
			break;
		}

		int i = 0;
		for (i = 0;i < nready;i ++) {



			if (eventloop->events[i].events & EPOLLIN) {
				//printf("sockitem\n");
				//还原当前的fd 设置的回调函数并触发
				struct sockitem *si = (struct sockitem*)eventloop->events[i].data.ptr;
				//出发对应的回调函数 也就是当前fd 用户态设置的回调函数 //保存触发事件的某个文件描述符相关的数据
				si->callback(si->sockfd, eventloop->events[i].events, si);

			}
            
			if (eventloop->events[i].events & EPOLLOUT) {
               
				struct sockitem *si = (struct sockitem*)eventloop->events[i].data.ptr;
				si->callback(si->sockfd, eventloop->events[i].events, si);

			}
		}

	}

}




