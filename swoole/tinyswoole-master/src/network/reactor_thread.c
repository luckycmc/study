#include "epoll.h"
#include "log.h"
#include "server.h"
#include "tinyswoole.h"
#include "process_pool.h"
//reactor 线程进入事件循环
static int tswReactorThread_loop(tswThreadParam *param)
{
    int pti = param->pti;   int i;
    tswServer *serv = param->object;
    tswReactor *reactor = &(serv->reactor_threads[pti].reactor);

    reactor->id = pti;
    while (1) {
        int nfds;

        nfds = reactor->wait(reactor);  // 获取当前线程的就绪fd 或者说事件
        if (nfds < 0) {
            tswWarn("%s", "reactor thread epoll wait error");
            return TSW_ERR;
        }

        for (i = 0; i < nfds; i++) {
            int connfd;
            tswReactorEpoll *reactor_epoll_object = reactor->object;

            tswEvent *tswev = (tswEvent *)reactor_epoll_object->events[i].data.ptr;
            tswDebug("reactor thread [%d] handler the event", reactor->id);
            //处理对用绑定 回调函数
            if (tswev->event_handler(reactor, tswev) < 0) {
                tswWarn("%s", "event_handler error");
                continue;
            }
        }
    }
}
//reactor线程组申请内存
int tswReactorThread_create(tswServer *serv)
{
    tswReactorThread *thread; int i;

    serv->reactor_threads = (tswReactorThread *)malloc(sizeof(tswReactorThread) * serv->reactor_num);
    if (serv->reactor_threads == NULL) {
        tswWarn("%s", "malloc error");
        return TSW_ERR;
    }
    
    for (i = 0; i < serv->reactor_num; i++) {
        thread = &(serv->reactor_threads[i]);
        // 线程对应的reactor初始化
        if (tswReactor_create(&(thread->reactor), MAXEVENTS) < 0) {
            tswWarn("%s", "tswReactor_create error");
            return TSW_ERR;
        }
    }

    return TSW_OK;
}
// reactor 线程启动
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
        if (pthread_create(&pidt, NULL, (void * (*)(void *))tswReactorThread_loop, (void *)param) < 0) {
            tswWarn("%s", "pthread_create error");
        }
        thread->thread_id = pidt;
    }

    return TSW_OK;
}
// reactor 线程把数据发送给 对应的worker进程
int tswReactorThread_sendToWorker(tswServer *serv, tswEventData *event_data, int worker_id)
{
    int pipe_master;
    tswReactor *reactor;

    pipe_master = serv->process_pool->workers[worker_id].pipe_master;
    // 写入到对应进程 对应的 pipe_master 管道中
    write(pipe_master, (void *)event_data, sizeof(event_data->info) + event_data->info.len);

    reactor = &(serv->reactor_threads[event_data->info.from_id].reactor);
    // 注册事件 接受worker 进程发送的数据 等待事件的到来
    // 也就是说 检测到  pipe_master 管道中的数据可以读时 回调函数 tswReactorThread_onPipeReceive
    if (reactor->add(reactor, pipe_master, TSW_EVENT_READ, tswReactorThread_onPipeReceive) < 0) {
        tswWarn("%s", "reactor add error");
        return TSW_ERR;
    }

    return TSW_OK;
}
//接受worker 发送的数据 发送给客户端（reactor事件就绪 会回调次函数 发数据发送给客户端）
int tswReactorThread_onPipeReceive(tswReactor *reactor, tswEvent *tswev)
{
    int n;
    int session_id;
    int connfd;
    tswEventData event_data;
    tswSession *session;

    // tswev->fd represents the fd of the pipe 
    // 读取worker 进程发送过来的数据
    n = read(tswev->fd, &event_data, sizeof(event_data));
    session_id = event_data.info.fd;
    session = &(TSwooleG.serv->session_list[session_id]);
    // 发送数据给客户端
    n = send(session->connfd, event_data.data, event_data.info.len, 0);
     //清除对应的fd不在进行 reactor监控
    if (reactor->del(reactor, tswev->fd) < 0) {
            tswWarn("%s", "reactor del error");
            return TSW_ERR;
    }
    //触发关闭事件
    tswServer_master_onClose();
    /********后续自己添加的start******/    
    /*if(n == -1){
         printf("client is closed\n");
         //删除对应的fd 不仅进行通讯
        if (reactor->del(reactor, tswev->fd) < 0) {
            tswWarn("%s", "reactor del error");
            return TSW_ERR;
        }
    }else if(n > 0){
          //数据发送完 等待fd在下次送数据
        if (reactor->set(reactor, tswev->fd,TSW_EVENT_READ) < 0) {
            tswWarn("%s", "reactor del error");
            return TSW_ERR;
        }
    }*/
     /********后续自己添加的end******/  

    return TSW_OK;
}