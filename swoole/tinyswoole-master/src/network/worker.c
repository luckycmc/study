#include "worker.h"
#include "server.h"
#include "log.h"
//接收到 用户的数据 也就是 reactor线程转发过来的数据
static int tswWorker_onPipeReceive(tswReactor *reactor, tswEvent *tswev)
{
    int n;
    tswEventData event_data;

    // tswev->fd represents the pipe_worker 
    n = read(tswev->fd, &event_data, sizeof(event_data));
    // 触发 onReceive 回调函数
    if (event_data.info.len > 0) {
        TSwooleG.serv->onReceive(TSwooleG.serv, &event_data);
    }

    return TSW_OK;
}
//worker 进程 转发数据到 reactor线程 reactor 线程转发数据给客户端
int tswWorker_sendToReactor(tswEventData *event_data)
{    
    printf("crrrent pid is %d\n",TSwooleWG.id);
    write(TSwooleWG.pipe_worker, event_data, sizeof(event_data->info) + event_data->info.len);
    return TSW_OK;
}
// 工作进程进入事件循环依靠 epoll 监听数据是否 可读或者可写
int tswWorker_loop()
{
    tswReactor *main_reactor; int i;

    main_reactor = malloc(sizeof(tswReactor));
    if (main_reactor == NULL) {
        tswWarn("%s", "malloc error");
        return TSW_ERR;
    }

    if (tswReactor_create(main_reactor, MAXEVENTS) < 0) {
        tswWarn("%s", "tswReactor_create error");
        return TSW_ERR;
    }
    // 注册reactor事件 回调函数是 tswWorker_onPipeReceive
    if (main_reactor->add(main_reactor, TSwooleWG.pipe_worker, TSW_EVENT_READ, tswWorker_onPipeReceive) < 0) {
        tswWarn("%s", "reactor add error");
        return TSW_ERR;
    }

    while (1) {
        int nfds;
        //等待就绪事件的返回
        nfds = main_reactor->wait(main_reactor);
        if (nfds < 0) {
            tswWarn("%s", "master thread epoll wait error");
            return TSW_ERR;
        }
        
        for (i = 0; i < nfds; i++) {
            int connfd;
            tswReactorEpoll *reactor_epoll_object = main_reactor->object;

            tswEvent *tswev = (tswEvent *)reactor_epoll_object->events[i].data.ptr;
            tswDebug("worker process [%d] handle the data", TSwooleWG.id);
            // 回调函数的执行
            if (tswev->event_handler(main_reactor, tswev) < 0) {
                tswWarn("%s", "event_handler error");
                continue;
            }
        }
    }
}
