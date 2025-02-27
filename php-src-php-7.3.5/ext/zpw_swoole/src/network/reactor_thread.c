#include "epoll.h"
#include "log.h"
#include "server.h"
#include "tinyswoole.h"
#include "process_pool.h"
/******
 * reactor 线程的事件循环
 * */
static int tswReactorThread_loop(tswThreadParam *param)
{
    int pti = param->pti;
    tswServer *serv = param->object;
    tswReactor *reactor = &(serv->reactor_threads[pti].reactor);

    reactor->id = pti;
    while (1) {
        int nfds;

        nfds = reactor->wait(reactor);
        if (nfds < 0) {
            tswWarn("%s", "reactor thread epoll wait error");
            return TSW_ERR;
        }
       int i;
        for (i = 0; i < nfds; i++) {
            int connfd;
            tswReactorEpoll *reactor_epoll_object = reactor->object;

            tswEvent *tswev = (tswEvent *)reactor_epoll_object->events[i].data.ptr;
            tswDebug("reactor thread [%d] handler the event", reactor->id);
            if (tswev->event_handler(reactor, tswev) < 0) {
                tswWarn("%s", "event_handler error");
                continue;
            }
        }
    }
}
// reacor 线程启动 根据reactor线程创建对应的 reactor模型
int tswReactorThread_create(tswServer *serv)
{
    tswReactorThread *thread;

    serv->reactor_threads = (tswReactorThread *)malloc(sizeof(tswReactorThread) * serv->reactor_num);
    if (serv->reactor_threads == NULL) {
        tswWarn("%s", "malloc error");
        return TSW_ERR;
    }
    int i;
    for (i = 0; i < serv->reactor_num; i++) {
        thread = &(serv->reactor_threads[i]);
        if (tswReactor_create(&(thread->reactor), MAXEVENTS) < 0) {
            tswWarn("%s", "tswReactor_create error");
            return TSW_ERR;
        }
    }

    return TSW_OK;
}
/******
 * reactor 线程启动 并且进入事件循环
 */
int tswReactorThread_start(tswServer *serv)
{
    pthread_t pidt;
    tswReactorThread *thread;
    tswThreadParam *param;
    int i;
    for (i = 0; i < serv->reactor_num; i++) {
        param = (tswThreadParam *)malloc(sizeof(tswThreadParam));
        if (param == NULL) {
            tswWarn("%s", "malloc error");
            return TSW_ERR;
        }

        thread = &(serv->reactor_threads[i]);
        param->pti = i;
        param->object = serv;

        serv->onReactorStart(i);
        //创建对应的线程 进入事件循环
        if (pthread_create(&pidt, NULL, (void * (*)(void *))tswReactorThread_loop, (void *)param) < 0) {
            tswWarn("%s", "pthread_create error");
        }
        thread->thread_id = pidt;
    }

    return TSW_OK;
}
/******
 *  reactor 线程把数据 投递给worker 进程 并加入到事件循环中
 * */
int tswReactorThread_sendToWorker(tswServer *serv, tswEventData *event_data, int worker_id)
{
    int pipe_master;    // 进程下的proc/进程/fd/的 
    tswReactor *reactor;

    pipe_master = serv->process_pool->workers[worker_id].pipe_master;
    write(pipe_master, (void *)event_data, sizeof(event_data->info) + event_data->info.len);

    reactor = &(serv->reactor_threads[event_data->info.from_id].reactor);
    //设置对应的回调函数 tswReactorThread_onPipeReceive worker进程处理好的数据 投递给客户端
    if (reactor->add(reactor, pipe_master, TSW_EVENT_READ, tswReactorThread_onPipeReceive) < 0) {
        tswWarn("%s", "reactor add error");
        return TSW_ERR;
    }

    return TSW_OK;
}
/*******
 *  接受worker 进程发送过来的数据 投递给用户
 */
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

    send(session->connfd, event_data.data, event_data.info.len, 0);
    if (reactor->del(reactor, tswev->fd) < 0) {
        tswWarn("%s", "reactor del error");
        return TSW_ERR;
    }

    return TSW_OK;
}