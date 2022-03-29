#include "worker.h"
#include "server.h"
#include "log.h"
// worker 进程管道接受 数据
static int tswWorker_onPipeReceive(tswReactor *reactor, tswEvent *tswev)
{
    int n;
    tswEventData event_data;
     printf ("tswev->fd is %d\n", tswev->fd);
    // tswev->fd represents the pipe_worker
    n = read(tswev->fd, &event_data, sizeof(event_data));
    if (event_data.info.len > 0) {
        TSwooleG.serv->onReceive(TSwooleG.serv, &event_data); //出发回调事件
    }

    return TSW_OK;
}
//worker进程把数据转发给 reactor线程
int tswWorker_sendToReactor(tswEventData *event_data)
{   
    //printf("currrnt fd is %d\n",event_data->info.fd);
    write(TSwooleWG.pipe_worker, event_data, sizeof(event_data->info) + event_data->info.len);
    return TSW_OK;
}
//worker 进程进入事件循环
int tswWorker_loop()
{
    tswReactor *main_reactor;

    main_reactor = malloc(sizeof(tswReactor));
    if (main_reactor == NULL) {
        tswWarn("%s", "malloc error");
        return TSW_ERR;
    }

    if (tswReactor_create(main_reactor, MAXEVENTS) < 0) {
        tswWarn("%s", "tswReactor_create error");
        return TSW_ERR;
    }
    //当前的 fd加入epoll_add 
    if (main_reactor->add(main_reactor, TSwooleWG.pipe_worker, TSW_EVENT_READ, tswWorker_onPipeReceive) < 0) {
        tswWarn("%s", "reactor add error");
        return TSW_ERR;
    }
    
    while (1) {
        int nfds;

        nfds = main_reactor->wait(main_reactor);
        if (nfds < 0) {
            tswWarn("%s", "master thread epoll wait error");
            return TSW_ERR;
        }
        int i;
        for (i = 0; i < nfds; i++) {
            int connfd;
            tswReactorEpoll *reactor_epoll_object = main_reactor->object;

            tswEvent *tswev = (tswEvent *)reactor_epoll_object->events[i].data.ptr;
            tswDebug("worker process [%d] handle the data", TSwooleWG.id);
            if (tswev->event_handler(main_reactor, tswev) < 0) {
                tswWarn("%s", "event_handler error");
                continue;
            }
        }
    }
}
