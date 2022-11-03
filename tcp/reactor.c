#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <pthread.h>

#include <errno.h>
#include <sys/epoll.h>

struct sockitem { //  socket item 对应的节点 
	int sockfd;
	int (*callback)(int fd, int events, void *arg);  //指针回调函数

	char recvbuffer[1024]; //
	char sendbuffer[1024];

};

// mainloop / eventloop --> epoll -->  
struct reactor {

	int epfd;
	struct epoll_event events[512];

};


struct reactor *eventloop = NULL;


int recv_cb(int fd, int events, void *arg);

//发送数据给 
int send_cb(int fd, int events, void *arg) {
    
	struct sockitem *si = (struct sockitem*)arg;
    printf("send data is success:\n");

	send(fd, "hello\n", 6, 0); //

	struct epoll_event ev;
	ev.events = EPOLLIN | EPOLLET;
	//ev.data.fd = clientfd;
	si->sockfd = fd;
	si->callback = recv_cb;
	ev.data.ptr = si;
    //  fd 的属性被修改了
	epoll_ctl(eventloop->epfd, EPOLL_CTL_MOD, fd, &ev);

}

//  ./epoll 8080
//  接受对应的数据
int recv_cb(int fd, int events, void *arg) {

	//int clientfd = events[i].data.fd;
	struct sockitem *si = (struct sockitem*)arg;
	struct epoll_event ev;

	char buffer[1024] = {0};
    //读取客户端的数据
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
		

	} else if (ret == 0) { //关闭客户端

		// 
		printf("disconnect %d\n", fd);

		ev.events = EPOLLIN;
		//ev.data.fd = fd;
		epoll_ctl(eventloop->epfd, EPOLL_CTL_DEL, fd, &ev);

		close(fd);

		free(si);
		
	} else {
        // 代码 提示收到客户端的数据
		printf("Recv: %s, %d Bytes\n", buffer, ret);

		// fd --> full 
		// fd epollout 
		//ret = send(fd, buffer, ret, 0);
		//if (ret == -1) {

		// }
		// 
		// 

		struct epoll_event ev;
		ev.events = EPOLLOUT | EPOLLET;
		//ev.data.fd = clientfd;
		si->sockfd = fd;
		si->callback = send_cb;
		ev.data.ptr = si;

		epoll_ctl(eventloop->epfd, EPOLL_CTL_MOD, fd, &ev);

	}

}

//接受客户端连接 是否有连接的到来
int accept_cb(int fd, int events, void *arg) {

	struct sockaddr_in client_addr;
	memset(&client_addr, 0, sizeof(struct sockaddr_in));
	socklen_t client_len = sizeof(client_addr);
	//接受客户端连接
	int clientfd = accept(fd, (struct sockaddr*)&client_addr, &client_len);
	if (clientfd <= 0) return -1;

	char str[INET_ADDRSTRLEN] = {0};
    // 提示客户端信息
	printf("recv from %s at port %d\n", inet_ntop(AF_INET, &client_addr.sin_addr, str, sizeof(str)),
		ntohs(client_addr.sin_port));

	struct epoll_event ev;
	ev.events = EPOLLIN | EPOLLET;  // 设置水平触发
	//ev.data.fd = clientfd;

	struct sockitem *si = (struct sockitem*)malloc(sizeof(struct sockitem));
	si->sockfd = clientfd;
	si->callback = recv_cb; // 回调接受客户端的数据
	ev.data.ptr = si;
	
	epoll_ctl(eventloop->epfd, EPOLL_CTL_ADD, clientfd, &ev);
	
	return clientfd;
}

int main(int argc, char *argv[]) {

	if (argc < 2) {
		return -1;
	}

	int port = atoi(argv[1]);  //获取对应的端口号

	//socket

	int sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd < 0) {
		return -1;
	}

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(struct sockaddr_in));

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = INADDR_ANY;
    //bind 
	if (bind(sockfd, (struct sockaddr*)&addr, sizeof(struct sockaddr_in)) < 0) {
		return -2;
	}
    //listen  监听
	if (listen(sockfd, 5) < 0) {
		return -3;
	}

	eventloop = (struct reactor*)malloc(sizeof(struct reactor));
	// epoll opera

	eventloop->epfd = epoll_create(1);
	
	struct epoll_event ev;  //创建event 事件
	ev.events = EPOLLIN;
	//ev.data.fd = sockfd; //int idx = 2000;
	

	struct sockitem *si = (struct sockitem*)malloc(sizeof(struct sockitem));
	si->sockfd = sockfd;
	si->callback = accept_cb;   //  指向的对应的回调函数  一旦有事件触发
	ev.data.ptr = si;  // si 加入对应的 ev.data.ptr
	
	int result = epoll_ctl(eventloop->epfd, EPOLL_CTL_ADD, sockfd, &ev);   //加入对应的epoll_ctl
    //验证是否加入成功
	if(result == -1){
          
           return -1;
    }

	//pthread_cond_waittime();
	//阻塞 等待 客户端连接
	while (1) {
        
        // 有数据时会触发  
		int nready = epoll_wait(eventloop->epfd, eventloop->events, 512, -1);
		if (nready < -1) {
			break;
		}
        //printf("有数据进来了%d\n",nready);
		int i = 0;
		for (i = 0;i < nready;i++) {


            //数据可读
			if (eventloop->events[i].events & EPOLLIN) {
				//printf("sockitem\n");
				struct sockitem *si = (struct sockitem*)eventloop->events[i].data.ptr;
                // 出发对应的回调函数
				si->callback(si->sockfd, eventloop->events[i].events, si);

			}
            //数据可写
			if (eventloop->events[i].events & EPOLLOUT) {

				struct sockitem *si = (struct sockitem*)eventloop->events[i].data.ptr;
                // 出发对应的回调函数
				si->callback(si->sockfd, eventloop->events[i].events, si);

			}


		}

	}

}




