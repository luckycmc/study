#include "tswoole_config.h"
#include "process_pool.h"
#include "worker.h"
#include "log.h"
#include "server.h"

tswServer *tswServer_new(void)
{
    tswServer *serv;

    serv = (tswServer *)malloc(sizeof(tswServer));
    if (serv == NULL) {
        return NULL;
    }
    serv->onStart = NULL;
    serv->onConnect = NULL;
    serv->onReceive = NULL;
    serv->onClose = NULL;
    serv->onReactorStart = NULL;

    serv->connection_list = (tswConnection *)malloc(sizeof(tswConnection) * TSW_CONNECTION_LIST_SIZE);
    if (serv->connection_list == NULL) {
        tswWarn("%s", "malloc error");
        return NULL;
    }

    serv->session_list = (tswSession *)malloc(sizeof(tswSession) * TSW_SESSION_LIST_SIZE);
    if (serv->session_list == NULL) {
        tswWarn("%s", "malloc error");
        return NULL;
    }

    serv->status = (tswServerStatus *)malloc(sizeof(tswServerStatus));
    if (serv->status == NULL) {
        tswWarn("%s", "malloc error");
        return NULL;
    }
    serv->status->accept_count = 0;

    return serv;
}

/*
  创建reactor线程
 * Create reactor threads
*/
static int tswServer_start_proxy(tswServer *serv)
{
    tswReactor *main_reactor;

    serv->reactor_pipe_num = serv->worker_num / serv->reactor_num;

    main_reactor = malloc(sizeof(tswReactor));
    if (main_reactor == NULL) {
        tswWarn("%s", "malloc error");
        return TSW_ERR;
    }
    // main reactor 的创建
    if (tswReactor_create(main_reactor, MAXEVENTS) < 0) {
        tswWarn("%s", "tswReactor_create error");
        return TSW_ERR;
    }
    //reactor线程的创建
    if (tswReactorThread_create(serv) < 0) {
        tswWarn("%s", "tswReactorThread_create error");
        return TSW_ERR;
    }
    //reactor线程的启动
    if (tswReactorThread_start(serv) < 0) {
        tswWarn("%s", "tswReactorThread_start error");
        return TSW_ERR;
    }

    if (listen(serv->serv_sock, LISTENQ) < 0) {
        tswWarn("%s", strerror(errno));
    }
    if (serv->onStart != NULL) {
        serv->onStart(serv);
    }
    //主线程进入事件循环 监听 listen fd 回调 accpet的函数 
    if (main_reactor->add(main_reactor, serv->serv_sock, TSW_EVENT_READ, tswServer_master_onAccept) < 0) {
        tswWarn("%s", "reactor add error");
        return TSW_ERR;
    }

    for (;;) {
        int nfds;

        nfds = main_reactor->wait(main_reactor);
        if (nfds < 0) {
            tswWarn("%s", "master thread epoll wait error");
            return TSW_ERR;
        }
         int i;
        for (i = 0; i < nfds; i++) {
            tswReactorThread *tsw_reactor_thread;
            tswReactorEpoll *reactor_epoll_object = main_reactor->object;
            //获取对的fd 事件然后 触发对应的回调函数
            tswEvent *tswev = (tswEvent *)reactor_epoll_object->events[i].data.ptr;
            tswDebug("%s", "master thread handler the event");
            if (tswev->event_handler(main_reactor, tswev) < 0) {
                tswWarn("%s", "event_handler error");
                continue;
            }
        }
    }

    close(serv->serv_sock); // 关闭listenfd

    return TSW_OK;
}
// 服务器启动
int tswServer_start(tswServer *serv)
{
    tswProcessPool *pool;
    tswPipe *pipe;

    serv->onMasterStart(); //回调onMaster进程回调
    pool = (tswProcessPool *)malloc(sizeof(tswProcessPool));
    if (pool == NULL) {
        tswWarn("%s", "malloc error");
        return TSW_ERR;
    }
    if (tswProcessPool_create(pool, serv->worker_num) < 0) {
        tswWarn("%s", "tswProcessPool_create error");
        return TSW_ERR;
    }
    int i;
    for ( i = 0; i < serv->worker_num; i++) {
        tswPipeUnsock *object;

        pipe = &pool->pipes[i];
        //通讯管道的创建
        if (tswPipeUnsock_create(pipe) < 0) {
            tswWarn("%s", "tswPipeUnsock_create error");
            return TSW_ERR;
        }
        //主进程 和工作进程 管道的创建  用于进程之间的通讯
        pool->workers[i].pipe_master = pipe->getFd(pipe, TSW_PIPE_MASTER);
        pool->workers[i].pipe_worker = pipe->getFd(pipe, TSW_PIPE_WORKER);
        pool->workers[i].pipe_object = pipe;
    }
    //工作进程的创建  
    for (i = 0; i < serv->worker_num; i++) {
        if (tswServer_create_worker(serv, pool, i) < 0) {
            tswWarn("%s", "tswServer_create_worker error");
            return TSW_ERR;
        }
    }

    tswProcessPool_info(pool); //进程池的相关信息
    serv->process_pool = pool;

    if (tswServer_start_proxy(serv) < 0) {
        tswWarn("%s", "tswServer_start_proxy error");
        return TSW_ERR;
    }

    return TSW_OK;
}

/*
   reactor 线程 来接受对应的 连接  accept
 * reactor: Used to manage handle in tswEvent
*/
int tswServer_master_onAccept(tswReactor *reactor, tswEvent *tswev)
{
    int connfd;
    socklen_t len;
    struct sockaddr_in cliaddr;
    tswServer *serv = reactor->ptr;
    tswReactor *sub_reactor;

    len = sizeof(cliaddr);
    //接受客户端的连接
    connfd = accept(tswev->fd, (struct sockaddr *)&cliaddr, &len);
    if (connfd < 0) {
        tswWarn("%s", "accept error");
        return TSW_ERR;
    }

    serv->status->accept_count++;
   //取模的方式获取对应的reactor 去处理
    sub_reactor = &(serv->reactor_threads[connfd % serv->reactor_num].reactor);

    serv->connection_list[connfd].connfd = connfd;
    serv->connection_list[connfd].session_id = serv->status->accept_count;
    serv->connection_list[connfd].from_reactor_id = sub_reactor->id;
    serv->connection_list[connfd].serv_sock = serv->serv_sock;

    serv->session_list[serv->status->accept_count].session_id = serv->status->accept_count;
    serv->session_list[serv->status->accept_count].connfd = connfd;
    serv->session_list[serv->status->accept_count].reactor_id = sub_reactor->id;
    serv->session_list[serv->status->accept_count].serv_sock = serv->serv_sock;
    //触发对应的连接事件
    serv->onConnect(serv->status->accept_count);
   /***注册到对应的 sub_reactor reactor线程中去**/
    if (sub_reactor->add(sub_reactor, connfd, TSW_EVENT_READ, tswServer_reactor_onReceive) < 0) {
        tswWarn("%s", "reactor add error");
        return TSW_ERR;
    }

    return TSW_OK;
}
/**
 * recator 线程接受数据 并且投递给 worker线程
 * 
 * @param reactor 
 * @param tswev 
 * @return int 
 */
int tswServer_reactor_onReceive(tswReactor *reactor, tswEvent *tswev)
{
    int n;
    int pipe_master;
    char buffer[MAX_BUF_SIZE];
    tswReactorEpoll *reactor_epoll_object = reactor->object;
    tswEventData event_data;
    int worker_id;

    n = recv(tswev->fd, event_data.data, TSW_BUFFER_SIZE, 0);
    if (n == 0) {
        reactor->del(reactor, tswev->fd);
        close(tswev->fd);
        free(tswev);
        reactor->event_num -= 1;
        return TSW_OK;
    }else if (n < 0){  //出现错误
    
        rerturn TSW_ERR;
    }
    
    event_data.info.len = n;  //数据包的长度
    event_data.info.from_id = reactor->id; //reactor 的线程id
    event_data.info.fd = TSwooleG.serv->connection_list[tswev->fd].session_id; //对应的fd
    worker_id = tswev->fd % TSwooleG.serv->process_pool->worker_num; //投递给的worker进程
    //数据投递给对应的worker 进程
    if (tswReactorThread_sendToWorker(TSwooleG.serv, &event_data, worker_id) < 0) {
        tswWarn("%s", "tswReactorThread_sendToWorker error");
        return TSW_ERR;
    }

    return TSW_OK;
}

void tswServer_master_onStart(void)
{
    tswDebug("%s", "master thread started successfully");
}

void tswServer_reactor_onStart(int reactor_id)
{
    tswDebug("reactor thread [%d] started successfully", reactor_id);
}

/*
 * worker process: 
 * 
 * @fd: session_id
 * @data: data from the send() method of php user space
*/
int tswServer_tcp_send(tswServer *serv, int fd, const void *data, size_t length)
{
    tswEventData event_data;

    event_data.info.len = length;
    event_data.info.from_id = TSwooleWG.id;
    event_data.info.fd = fd;
    memcpy(event_data.data, data, length);

    tswWorker_sendToReactor(&event_data);
    return TSW_OK;
}
