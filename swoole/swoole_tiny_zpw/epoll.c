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
//线程epoll
void *worker_thread(void *arg);
int recv_cb(int fd, int events, void *arg);
int recv_cb(int fd, int events, void *arg);
int accept_cb(int fd, int events, void *arg);

struct reactor *eventloop = NULL;

int send_cb(int fd, int events, void *arg) 
{

	struct sockitem *si = (struct sockitem*)arg;

	send(fd, "hello\n", 6, 0); //

	struct epoll_event ev;
	ev.events = EPOLLIN | EPOLLET;
	//ev.data.fd = clientfd;
	si->sockfd = fd;
	si->callback = recv_cb;
	ev.data.ptr = si;

	epoll_ctl(eventloop->epfd, EPOLL_CTL_MOD, fd, &ev);
     //回调onFinish();函数
      onFinish();
}

//  ./epoll 8080

int recv_cb(int fd, int events, void *arg) 
{

	//int clientfd = events[i].data.fd;
	struct sockitem *si = (struct sockitem*)arg;
	struct epoll_event ev;

	char buffer[1024] = {0};
	int ret = recv(fd, buffer, 1024, 0);
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

          onClose();
		
	} else {

		printf("Recv: %s, %d Bytes\n", buffer, ret);

		struct epoll_event ev;
		ev.events = EPOLLOUT | EPOLLET;
		//ev.data.fd = clientfd;
		si->sockfd = fd;
		si->callback = send_cb;
		ev.data.ptr = si;
		epoll_ctl(eventloop->epfd, EPOLL_CTL_MOD, fd, &ev);
          onReceive();
	}

}


int accept_cb(int fd, int events, void *arg) 
{

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
	// 出发回调函数
     onConnect();
	return clientfd;
}


//启动一个线程
void *worker_thread(void *arg) 
{

	while (1) 
     {
		int nready = epoll_wait(eventloop->epfd, eventloop->events, 512, -1);

		if (nready < -1) {
			break;
		}else if (nready == 0)
          {
              continue;  //进入下一轮事件循环
          }
          
		int i = 0;
          //处理完善的IO
		for (i = 0;i < nready;i ++) 
          {

			if (eventloop->events[i].events & EPOLLIN) {
				//printf("sockitem\n");
				struct sockitem *si = (struct sockitem*)eventloop->events[i].data.ptr;
				si->callback(si->sockfd, eventloop->events[i].events, si);
			}

			if (eventloop->events[i].events & EPOLLOUT) {

				struct sockitem *si = (struct sockitem*)eventloop->events[i].data.ptr;
				si->callback(si->sockfd, eventloop->events[i].events, si);
			}
		}
	}
}

int main(int argc, char *argv[]) 
{

	int sockfd = listenFd();

	printf("server is starting\n");

	eventloop = (struct reactor*)malloc(sizeof(struct reactor));
	// epoll opera

	eventloop->epfd = epoll_create(1);


	struct epoll_event ev;
	ev.events = EPOLLIN;
	//ev.data.fd = sockfd; //int idx = 2000;
	
	struct sockitem *si = (struct sockitem*)malloc(sizeof(struct sockitem));
	si->sockfd = sockfd;
	si->callback = accept_cb;
	ev.data.ptr = si;
	
	epoll_ctl(eventloop->epfd, EPOLL_CTL_ADD, sockfd, &ev);

	pthread_t id;
	pthread_create(&id, NULL, worker_thread, NULL);
	
	//pthread_cond_waittime();
	while(1) {

		

	}
	
}
