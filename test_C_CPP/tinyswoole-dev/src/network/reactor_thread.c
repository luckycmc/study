#include "epoll.h"
#include "log.h"
#include "server.h"
#include "tinyswoole.h"
#include "process_pool.h"
//线程进入事件循环
static int tswReactorThread_loop(tswThreadParam *param)
{
    int pti = param->pti;
    tswServer *serv = param->object;
    tswReactor *reactor = &(serv->reactor_threads[pti].reactor);

    reactor->id = pti;
    for (;;) {
        int nfds;

        nfds = reactor->wait(reactor);
        if (nfds < 0) {
            tswWarn("%s", "reactor thread epoll wait error");
            return TSW_ERR;
        }

        for (int i = 0; i < nfds; i++) {
            int connfd;
            tswReactorEpoll *reactor_epoll_object = reactor->object;
            //取出对应的处理函数
            tswEvent *tswev = (tswEvent *)reactor_epoll_object->events[i].data.ptr;
            tswDebug("reactor thread [%d] handler the event", reactor->id);
            //执行回调函数
            if (tswev->event_handler(reactor, tswev) < 0) {
                tswWarn("%s", "event_handler error");
                continue;
            }
        }
    }
}
//线程的创建
int tswReactorThread_create(tswServer *serv)
{
    tswReactorThread *thread;
    // 为reactor线程分配空间
    serv->reactor_threads = (tswReactorThread *)malloc(sizeof(tswReactorThread) * serv->reactor_num);
    if (serv->reactor_threads == NULL) {
        tswWarn("%s", "malloc error");
        return TSW_ERR;
    }
    //创建reactor线程进
    for (int i = 0; i < serv->reactor_num; i++) {
        thread = &(serv->reactor_threads[i]);
        if (tswReactor_create(&(thread->reactor), MAXEVENTS) < 0) {
            tswWarn("%s", "tswReactor_create error");
            return TSW_ERR;
        }
    }

    return TSW_OK;
}
//线程的启动
int tswReactorThread_start(tswServer *serv)
{
    pthread_t pidt;
    tswReactorThread *thread;
    tswThreadParam *param;

    for (int i = 0; i < serv->reactor_num; i++) {
        param = (tswThreadParam *)malloc(sizeof(tswThreadParam));
        if (param == NULL) {
            tswWarn("%s", "malloc error");
            return TSW_ERR;
        }

        thread = &(serv->reactor_threads[i]);
        param->pti = i;
        param->object = serv;

        serv->onReactorStart(i);
        if (pthread_create(&pidt, NULL, (void * (*)(void *))tswReactorThread_loop, (void *)param) < 0) {
            tswWarn("%s", "pthread_create error");
        }
        thread->thread_id = pidt;
    }

    return TSW_OK;
}
// reactor线程发送数据给worker进程
int tswReactorThread_sendToWorker(tswServer *serv, tswEventData *event_data, int worker_id)
{
    int pipe_master;
    tswReactor *reactor;

    pipe_master = serv->process_pool->workers[worker_id].pipe_master;
    write(pipe_master, (void *)event_data, sizeof(event_data->info) + event_data->info.len);

    reactor = &(serv->reactor_threads[event_data->info.from_id].reactor);
    if (reactor->add(reactor, pipe_master, TSW_EVENT_READ, tswReactorThread_onPipeReceive) < 0) {
        tswWarn("%s", "reactor add error");
        return TSW_ERR;
    }

    return TSW_OK;
}
//接受管道中的数据发送给对应的用户
int tswReactorThread_onPipeReceive(tswReactor *reactor, tswEvent *tswev)
{
    int n;
    int session_id;
    int connfd;
    tswEventData event_data;
    tswSession *session;

    // tswev->fd represents the fd of the pipe
    n = read(tswev->fd, &event_data, sizeof(event_data));
    session_id = event_data.info.fd;
    session = &(TSwooleG.serv->session_list[session_id]);
    //数据直接发送给用户
    send(session->connfd, event_data.data, event_data.info.len, 0);
    if (reactor->del(reactor, tswev->fd) < 0) {
        tswWarn("%s", "reactor del error");
        return TSW_ERR;
    }

    return TSW_OK;
}