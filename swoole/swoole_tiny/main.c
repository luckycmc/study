#include "./include/swoole.h"
#include "./include/Server.h"
/**
  1.连接的建立用两种 第一种是来自客户端的建立  epollin 表示此时事件 可读
                    第二种是服务器去连接其他的服务器  epollout 表示事件可写
  2.连接的断开
             客户端断开
			 服务器端的断开
  3.消息到达
       读缓存区的数据复制到用户空间中
	   read = 0 收到客户端关闭的FIN包
	   read > 0 正常的业务逻辑
	   read = -1  出现错误 （其中一个是tcp探活包）
  4.消息发送完毕

  业务逻辑与网络之间的处理 是否要分离
  
  listenfd
  acceptfd 


  accept函数返回值成功时返回非负值，失败时返回-1

 accept函数接受一个客户端请求后会返回一个新的SOCKFD值，当有不同的客户端同时有不同请求时，
 会返回不同的SOCKFD的值。这个不同的值和建立SOCKET 时生成的SOCKFD还是不同的。
 服务器与客户端之间的通信就是在这些不同的SOCKFD上进行的。

 在服务器端，socket()返回的套接字用于监听（listen）和接受（accept）客户端的连接请求。
 这个套接字不能用于与客户端之间发送和接收数据。accept()接受一个客户端的连接请求，
 并返回一个新的套接字。所谓“新的”就是说这个套接字与socket()返回的用于监听和接受客户端
 的连接请求的套接字不是同一个套接字。
 与本次接受的客户端的通信是通过在这个新的套接字上发送和接收数据来完成的。

 再次调用accept()可以接受下一个客户端的连接请求，并再次返回一个新的套接字（与socket()
 返回的套接字、之前accept()返回的套接字都不同的新的套接字）。
 这个新的套接字用于与这次接受的客户端之间的通信。假设一共有3个客户端连接到服务器端。
 那么在服务器端就一共有4个套接字：第1个是socket()返回的、用于监听的套接字；
 其余3个是分别调用3次accept()返回的不同的套接字。如果已经有客户端连接到服务器端，
 不再需要监听和接受更多的客户端连接的时候，可以关闭由socket()返回的套接字，
 而不会影响与客户端之间的通信。当某个客户端断开连接、或者是与某个客户端的通信完成之后，
 服务器端需要关闭用于与该客户端通信的套接字。
 
 swoole 主线程  主线程负责监听 server socket ，如果有新的accept ,主线程会评估reactor
 的连接数量。将此连接分配给连接数最少的reactor 线程，做一个负载均衡

 Reactor 线程组
    Reactor线程负责维护客户端机器的TCP连接、处理网络IO、收发数据完全是异步非阻塞的模式。

	swoole的主线程在Accept新的连接后，会将这个连接分配给一个固定的Reactor线程，
	在socket可读时读取数据，并进行协议解析，将请求投递到Worker进程。
	在socket可写时将数据发送给TCP客户端。

三次握手完成之后才调用accept

**/  

/*****初始化回调函数 start*****/
int my_onReceive(swFactory *factory, swEventData *req);
void my_onStart(swServer *serv);
void my_onShutdown(swServer *serv);
void my_onConnect(swServer *serv, int fd, int from_id);
void my_onClose(swServer *serv, int fd, int from_id);
void my_onTimer(swServer *serv, int interval);
void init_main(); //初始化
/**
  服务器端需要处理的数据 
    1.recv() 接受数据  
	2.parser()解析数据 
	3.send()发送数据到客户端
**/
/*****初始化回调函数 start*****/
int main()
{      
       //printf("server create start.....\n");
       //服务初始化
       init_main();

       return 0;
}
//初始话并且执行对应的回调函数
void init_main()
{
      swServer serv;

	   int ret;
      //服务初始化
	   swServer_init(&serv);

       //config
	  serv.backlog = 128;
	  serv.poll_thread_num = 1;
	  serv.writer_num = 1;       // 工作写进程数据量
	  serv.worker_num = 4;       //工作进程的数量
	  serv.factory_mode = 3;     // tcp 服务器
	  serv.open_cpu_affinity = 1; // 设置对应的CPU的亲和力
	  serv.open_tcp_nodelay = 1;
	  //serv.daemonize = 1;
      /************注册对应的回调事件 start*************/
      serv.onStart = my_onStart;         //服务启动的时候
	  serv.onShutdown = my_onShutdown;
	  serv.onConnect = my_onConnect;     //服务器被连接的时候
	  serv.onReceive = my_onReceive;     // 服务器接收到数据的时候
	  serv.onClose = my_onClose;         //服务器关闭的时候
	  serv.onTimer = my_onTimer;
      /************注册对应的回调事件 end*************/


      //创建server
      //create Server
	ret = swServer_create(&serv);
	if (ret < 0)
	{
		swTrace("create server fail[error=%d].\n", ret);
		exit(0);
	}
      //启动server
	ret = swServer_start(&serv);
	if (ret < 0)
	{
		swTrace("start server fail[error=%d].\n", ret);
		exit(0);
	}

}

void my_onTimer(swServer *serv, int interval)
{
	switch (interval)
	{
	case 1:
		printf("Timer[%d]\n", 1);
		break;
	case 10:
		printf("Timer[%d]\n", 10);
		break;
	default:
		break;
	}
}
//接受到数据被触发的事件
int my_onReceive(swFactory *factory, swEventData *req)
{
	int ret;
	char resp_data[SW_BUFFER_SIZE];
	swSendData resp;

	resp.fd = req->fd; //fd can be not source fd.
	resp.len = req->len + 8;
	resp.from_id = req->from_id;

	//swTrace("Data Len=%d %s\n", req->len, req->data);
	snprintf(resp_data, resp.len, "Server:%s", req->data);
	printf("my_onReceive 被触发的连接事件------------Server is running\n");
	resp.data = resp_data;
	ret = factory->finish(factory, &resp);
	if (ret < 0)
	{
		swWarn("send to client fail.errno=%d\n", errno);
	}
	swTrace("finish\n");
	return SW_OK;
}
//进程启动的时候
void my_onStart(swServer *serv)
{
	printf("my_onStart 被触发的连接事件------------Server is running\n");
}

void my_onShutdown(swServer *serv)
{
	printf("Server is shutdown\n");
}

void my_onConnect(swServer *serv, int fd, int from_id)
{
	printf("my_onConnect 被触发的连接事件-------------Connect fd=%d|from_id=%d\n", fd, from_id);
}

void my_onClose(swServer *serv, int fd, int from_id)
{
	printf("my_onClose 被触发的连接事件------------Close fd=%d|from_id=%d\n", fd, from_id);
}
//ps aux | grep swoole |  awk '{print $2}' | xargs kill -9 杀死所有对应的进程


// gdb -p 进程号 追中对应的进程
// thread  info 查看其中对应的线程

// bt 查看 堆栈

// t 2 表示查看的是线程2

// strace -f -p 进程号

