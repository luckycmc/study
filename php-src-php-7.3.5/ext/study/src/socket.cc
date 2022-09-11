#include "socket.h"
#include "log.h"
//创建socket 套接字

int stSocket_create(int domain, int type, int protocol)
{
    
     int sock;
    //创建套接字
    sock = socket(domain, type, protocol);
    if (sock < 0)
    {
        stWarn("Error has occurred: (errno %d) %s", errno, strerror(errno));
    }

    return sock;
}

//stSocket_bind bind 绑定
int stSocket_bind(int sock, int type, char *host, int port)
{
    int ret;
    struct sockaddr_in servaddr;

    if (type == ST_SOCK_TCP) // 创建TCP服务器
    {
        bzero(&servaddr, sizeof(servaddr));
        inet_aton(host, &(servaddr.sin_addr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(port);
        ret = bind(sock, (struct sockaddr *)&servaddr, sizeof(servaddr));
        if (ret < 0)
        {
            return -1; 
        }
    }
    else
    {
        return -1;
    }

    return ret;
}
//接受连接
int stSocket_accept(int sock)
{
    int connfd;
    struct sockaddr_in sa; // 对端的地址 结构体
    socklen_t len;

    len = sizeof(sa);
    //产生一个新的fd 进入全连接队列
    connfd = accept(sock, (struct sockaddr *)&sa, &len);
    
    return connfd; //通过这个fd可以和对端通讯
}
// server监听的封装  backlog 表示半连接队列的大小
int stSocket_listen(int sock,int backlog)
{
    int ret;

    ret = listen(sock, backlog);
    if (ret < 0)
    {
        stWarn("Error has occurred: (errno %d) %s", errno, strerror(errno));
    }
    return ret;
}
//接受客户端的数据
ssize_t stSocket_recv(int sock, void *buf, size_t len, int flag)
{
    ssize_t ret;

    ret = recv(sock, buf, len, flag);
    if (ret < 0 && errno != EAGAIN)
    {
        stWarn("Error has occurred: (errno %d) %s", errno, strerror(errno));
    }
    return ret;
}
//发送数据给对端发送数据
ssize_t stSocket_send(int sock, const void *buf, size_t len, int flag)
{
    ssize_t ret;

    ret = send(sock, buf, len, flag);
    if (ret < 0)
    {
        stWarn("Error has occurred: (errno %d) %s", errno, strerror(errno));
    }
    return ret;
}
//设置非阻塞的fd 以便于协成调度
int stSocket_set_nonblock(int sock)
{
    int flags;
    // 用来获取这个socket原来的一些属性。
    flags = fcntl(sock, F_GETFL, 0);
    if (flags < 0) {
        stWarn("Error has occurred: (errno %d) %s", errno, strerror(errno));
        return -1;
    }
    //用来在原来的属性上加上非阻塞的属性。
    flags = fcntl(sock, F_SETFL, flags | O_NONBLOCK);
    if (flags < 0) {
        stWarn("Error has occurred: (errno %d) %s", errno, strerror(errno));
        return -1;
    }
    return 0;
}
//实现关闭客户端
int stSocket_close(int fd)
{
    int ret;

    ret = close(fd);
    if (ret < 0)
    {
        stWarn("Error has occurred: (errno %d) %s", errno, strerror(errno));
    }
    return ret;
}





