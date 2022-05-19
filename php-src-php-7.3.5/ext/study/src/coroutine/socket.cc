#include "coroutine_socket.h"
#include "socket.h"

using Study::Coroutine::Socket;

//socket 的创建
Socket::Socket(int domain, int type, int protocol)
{
    sockfd = stSocket_create(domain, type, protocol);
    if (sockfd < 0)
    {
        return;
    }
    stSocket_set_nonblock(sockfd);
}
//cocket 绑定
int Socket::bind(int type, char *host, int port)
{
    return stSocket_bind(sockfd, type, host, port);
}
//socket 监听
int Socket::listen()
{
    return stSocket_listen(sockfd);
}
//socket 接受客户端连接
int Socket::accept()
{
    int connfd;

    connfd = stSocket_accept(sockfd);
    if (connfd < 0 && errno == EAGAIN)
    {
        wait_event(ST_EVENT_READ);
        connfd = stSocket_accept(sockfd);
    }

    return connfd;
}



