#include "coroutine_socket.h"
#include "socket.h"
#include "coroutine.h" // 增加的一行
using Study::Coroutine; // 增加的一行
using Study::coroutine::Socket; // 新增的一行

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
//等待事件到来
bool Socket::wait_event(int event)
{
    long id;
    Coroutine* co;
    epoll_event *ev;

    co = Coroutine::get_current(); //获取到当前的这个协程。
    id = co->get_cid();
    // 用来判断这个协程需要等待那种类型的事件，目前是支持READ和WRITE。
    ev = StudyG.poll->events;
    ev->events = event == ST_EVENT_READ ? EPOLLIN : EPOLLOUT;
    ev->data.u64 = touint64(sockfd, id);
    epoll_ctl(StudyG.poll->epollfd, EPOLL_CTL_ADD, sockfd, ev);

    co->yield();
    return true;
}
//接受客户端的数据
ssize_t Socket::recv(void *buf, size_t len)
{
    int ret;

    ret = stSocket_recv(sockfd, buf, len, 0);
    if (ret < 0 && errno == EAGAIN)
    {
        wait_event(ST_EVENT_READ);
        ret = stSocket_recv(sockfd, buf, len, 0);
    }
    return ret;
}
//发送数据给客户端
ssize_t Socket::send(const void *buf, size_t len)
{
    int ret;

    ret = stSocket_send(sockfd, buf, len, 0);
    if (ret < 0 && errno == EAGAIN)
    {
        wait_event(ST_EVENT_WRITE);
        ret = stSocket_send(sockfd, buf, len, 0);
    }
    return ret;
}
//关闭客户端
int Socket::close()
{
    return stSocket_close(sockfd);
}
//析构函数
Socket::~Socket()
{
}





