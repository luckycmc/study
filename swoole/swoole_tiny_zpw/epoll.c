#include "swoole_tiny_zpw.h"

#define BUFFER_LENGTH		1024

/*对应的函数*/
static int recv_cb(int fd, int events, void *arg);
static int send_cb(int fd, int events, void *arg);
static int accept_cb(int fd, int events, void *arg);

//保存Io状态
struct socketitem {

      int unsigned socket_fd;  //对应的fd
      //设置对应的回调函数
      int(* callback)(int fd,int events,void *arg);

      char recvbuffer[BUFFER_LENGTH]; //接受缓冲区的长度
	  char sendbuffer[BUFFER_LENGTH]; // 发送宋缓冲区的长度

	  int rlength;
	  int slength;
};

// 保存所有IO的全局变量
// mainloop / eventloop --> epoll -->  
struct reactor {
      
        int epfd; //红黑树的根节点
        struct epoll_event events[512];//对应的事件
};


struct reactor *eventloop = NULL;

