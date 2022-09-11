#include "coroutine_socket.h"
#include "coroutine.h"
#include "socket.h"
#include "log.h"

using study::Coroutine;
using study::coroutine::Socket;

char * Socket::read_buffer = nullptr;  // 增加的地方
size_t Socket::read_buffer_len = 0;     // 增加的地方
char * Socket::write_buffer = nullptr;  // 增加的地方
size_t Socket::write_buffer_len = 0;    // 增加的地方
/**
 * 所以，虽然通过协程，可以通过同步的方式编写高性能的服务器，但是，我们只有掌握了协程的原理，才可以避免一些坑。
 * 
 * @param domain 
 * @param type 
 * @param protocol 
 */
//创建socket
Socket::Socket(int domain, int type, int protocol)
{
   
    sockfd = stSocket_create(domain, type, protocol);
    if (sockfd < 0)
    {
        return;
    }
    //设置为非阻塞
    stSocket_set_nonblock(sockfd);
}
// 设置socket 为非阻塞
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

int Socket::listen(int backlog)
{
    return stSocket_listen(sockfd,backlog);
}
/***bind和listen这两个函数并不会阻塞 end***/
Socket* Socket::accept()
{
    int connfd;

    do
    {   
        //connfd < 0 && errno == EAGAIN 没有客户端连接 
        // 这是一个可读事件 一旦事件到来（此时，有客户端连接），我们的调度器就会去resume这个协程
        connfd = stSocket_accept(sockfd);

    } while (connfd < 0 && errno == EAGAIN && wait_event(ST_EVENT_READ));

     return (new Socket(connfd));
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
     // 加一个while 这是对协程化接口的一种同步，确保前面的函数执行成功了，再继续后面的函数。
    do
    {   
        //数据不可读则换出协成
        ret = stSocket_recv(sockfd, buf, len, 0);
    } while (ret < 0 && errno == EAGAIN && wait_event(ST_EVENT_READ));

    return ret;
}
//发送数据给对应的客户端 逻辑和recv一致
ssize_t Socket::send(const void *buf, size_t len)
{
    int ret;

    do
    {   
        // 数据是否准备完毕 准备完毕后 没有准备完毕 加入到IO多路复用的事件中
        ret = stSocket_send(sockfd, buf, len, 0);
    } while (ret < 0 && errno == EAGAIN && wait_event(ST_EVENT_WRITE));
    
    return ret;
}
//关闭对应的客户端
int Socket::close()
{
    return stSocket_close(sockfd);
}
//协成调度器 把协成和对应的事件进行绑定
bool Socket::wait_event(int event)
{
    long id;
    Coroutine* co;
    epoll_event *ev;
    /*******获取到当前对应的协程******/
    co = Coroutine::get_current();
    id = co->get_cid(); // 当前运行的协成id

    if (!StudyG.poll)
    {
        init_stPoll();
    }

    ev = StudyG.poll->events;
    //绑定对应的事件
    ev->events = event == ST_EVENT_READ ? EPOLLIN : EPOLLOUT;
    ev->data.u64 = touint64(sockfd, id);
    // 对应的fd 加入到对应epoll中
    epoll_ctl(StudyG.poll->epollfd, EPOLL_CTL_ADD, sockfd, ev);
    (StudyG.poll->event_num)++;
    // 让出当前协成
    co->yield();
     //这个地方可能存在问题需要处理  fd 的修改 修改对应的fd事件
    if (epoll_ctl(StudyG.poll->epollfd, EPOLL_CTL_DEL, sockfd, NULL) < 0)
    {
        stError("Error has occurred: (errno %d) %s", errno, strerror(errno));
        return false;
    }
    return true;
}
//初始化读缓冲区
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
//初始化写缓冲区
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