#include "coroutine_socket.h"
#include "coroutine.h"
#include "socket.h"
#include "log.h"

using study::Coroutine;
using study::coroutine::Socket;

char * Socket::read_buffer = nullptr;
size_t Socket::read_buffer_len = 0;
char * Socket::write_buffer = nullptr;
size_t Socket::write_buffer_len = 0;
//创建socket
Socket::Socket(int domain, int type, int protocol)
{
   
    sockfd = stSocket_create(domain, type, protocol);
    if (sockfd < 0)
    {
        return;
    }
    stSocket_set_nonblock(sockfd);
}

Socket::Socket(int fd)
{
    sockfd = fd;
    stSocket_set_nonblock(sockfd);
}

Socket::~Socket()
{
}
/***bind和listen这两个函数并不会阻塞 start***/
int Socket::bind(int type, char *host, int port)
{
    return stSocket_bind(sockfd, type, host, port);
}

int Socket::listen()
{
    return stSocket_listen(sockfd);
}
/***bind和listen这两个函数并不会阻塞 end***/
int Socket::accept()
{
    int connfd;

    do
    {   
        //connfd < 0 && errno == EAGAIN 没有客户端连接 这是一个可读事件
        // 一旦事件到来（此时，有客户端连接），我们的调度器就会去resume这个协程
        connfd = stSocket_accept(sockfd);

    } while (connfd < 0 && errno == EAGAIN && wait_event(ST_EVENT_READ));

    return connfd;
}
//读取数据
/*
  代码和协程化的accept的类似。先尝试着读取数据，如果不可以读取，那么就切换出这个协程。
  等到事件触发（也就是可读的时候），通过调度器resume这个协程，然后协程继续回到这个位置，
  然后执行第二次recv
*/
ssize_t Socket::recv(void *buf, size_t len)
{
    int ret;

    do
    {
        ret = stSocket_recv(sockfd, buf, len, 0);
    } while (ret < 0 && errno == EAGAIN && wait_event(ST_EVENT_READ));

    return ret;
}
//发送数据给对应的客户端
ssize_t Socket::send(const void *buf, size_t len)
{
    int ret;

    do
    {
        ret = stSocket_send(sockfd, buf, len, 0);
    } while (ret < 0 && errno == EAGAIN && wait_event(ST_EVENT_WRITE));
    
    return ret;
}
//关闭对应的客户端
int Socket::close()
{
    return stSocket_close(sockfd);
}
//协成调度器
bool Socket::wait_event(int event)
{
    long id;
    Coroutine* co;
    epoll_event *ev;

    co = Coroutine::get_current(); //获取到当前的这个协程
    id = co->get_cid();

    if (!StudyG.poll)
    {
        init_stPoll();
    }

    ev = StudyG.poll->events;

    ev->events = event == ST_EVENT_READ ? EPOLLIN : EPOLLOUT;
    ev->data.u64 = touint64(sockfd, id);
    //加入到epoll 中
    epoll_ctl(StudyG.poll->epollfd, EPOLL_CTL_ADD, sockfd, ev);
    (StudyG.poll->event_num)++;

    co->yield(); //切换当前协成

    if (epoll_ctl(StudyG.poll->epollfd, EPOLL_CTL_DEL, sockfd, NULL) < 0)
    {
        stError("Error has occurred: (errno %d) %s", errno, strerror(errno));
        return false;
    }
    return true;
}

int Socket::init_read_buffer()
{
    if (!read_buffer)
    {
        try
        {
            read_buffer = new char[65536];
        }
        catch(const std::bad_alloc& e)
        {
            stError("%s", e.what());
        }
        
        read_buffer_len = 65536;
    }

    return 0;
}

int Socket::init_write_buffer()
{
    if (!write_buffer)
    {
        try
        {
            write_buffer = new char[65536];
        }
        catch(const std::bad_alloc& e)
        {
            stError("%s", e.what());
        }
        
        write_buffer_len = 65536;
    }

    return 0;
}