#include "./include/swoole.h"

//接受客户端连接产生对应的fd
int swReactor_accept(swReactor *reactor, swEvent *event)
{  

	swEventConnect conn_ev;
	conn_ev.from_id = event->from_id;
	conn_ev.serv_fd = event->fd;
	conn_ev.addrlen = sizeof(conn_ev.addr);
	bzero(&conn_ev.addr, conn_ev.addrlen);

	conn_ev.conn_fd = accept(conn_ev.serv_fd, (struct sockaddr *) &conn_ev.addr, &conn_ev.addrlen);
	if (conn_ev.conn_fd < 0)
	{
		swTrace("[swReactorEpollWait]accept fail\n");
		return -1;
	}
	swSetNonBlock(conn_ev.conn_fd); //设置对应的fd
	//注册对应的连接fd 和对应的回调函数标识 
	reactor->add(reactor, conn_ev.conn_fd, SW_FD_TCP);

	return conn_ev.conn_fd;
}
// server  出现错误
inline int swReactor_error(swReactor *reactor)
{
	switch (errno)
	{
	case EINTR:
		return SW_OK;
	}
	return SW_ERR;
}
//客户端关闭连接
int swReactor_close(swReactor *reactor, swEvent *event)
{
	//swEventClose close_ev;
	//close_ev.fd = event->fd;
	//close_ev.from_id = event->fd;

	close(event->fd);  //关闭对应的fd节点
	reactor->del(reactor, event->fd);// 从reactor 也就是红黑树中 删除对应的fd
	return 0;
}
//不同的事件类型设置对应的回调函数
int swReactor_setHandle(swReactor *reactor, int fdtype, swReactor_handle handle)
{  
	//printf("swReactor_setHandle is %d\n",fdtype);
	//printf("hand is function is %p\n",handle);
	if (fdtype >= SW_MAX_FDTYPE)
	{
		return -1;
	}
	else
	{
		reactor->handle[fdtype] = handle; //对应的 文件描述符类型设置对应的回调函数
		return 0;
	}
}
//接受客户端发送的数据
int swReactor_receive(swReactor *reactor, swEvent *event)
{
	swEventData data;
	int ret;

	bzero(data.data, SW_BUFFER_SIZE);
	ret = swRead(event->fd, data.data, SW_BUFFER_SIZE);
	printf("[ReadThread]recv: %s|fd=%d|ret=%d|errno=%d\n", data.data, event->fd, ret, errno);

	if (ret == 0) //数据不同的阶段注册不同的回调函数   客户端关闭我们出发对应的关闭函数
	{
		//fd close
		event->type = SW_FD_CLOSE;
		return reactor->handle[SW_FD_CLOSE](reactor, event);
	}
	else if (ret > 0)
	{
		return reactor->handle[SW_FD_TCP](reactor, event); //正常接收数据触发对应的接受数据函数
	}
	else
	{
		swTrace("[swFactoryFunc_receive]epoll error\n");
		reactor->handle[SW_FD_ERROR](reactor, event);
		return -1;
	}
	return 0;
}
