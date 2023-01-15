#include "tswoole_config.h"
#include "process_pool.h"
#include "worker.h"
#include "log.h"
#include "server.h"
//服务器初始化
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
    tswReactor *main_reactor; int i;

    serv->reactor_pipe_num = serv->worker_num / serv->reactor_num;

    main_reactor = malloc(sizeof(tswReactor));
    if (main_reactor == NULL) {
        tswWarn("%s", "malloc error");
        return TSW_ERR;
    }
    //主线程的 创建reactor
    if (tswReactor_create(main_reactor, MAXEVENTS) < 0) {
        tswWarn("%s", "tswReactor_create error");
        return TSW_ERR;
    }
    // reactor 线程的创建 和初始化
    if (tswReactorThread_create(serv) < 0) {
        tswWarn("%s", "tswReactorThread_create error");
        return TSW_ERR;
    }
    // reactor 线程启动
    if (tswReactorThread_start(serv) < 0) {
        tswWarn("%s", "tswReactorThread_start error");
        return TSW_ERR;
    }
    //监听 对应的socket
    if (listen(serv->serv_sock, LISTENQ) < 0) {
        tswWarn("%s", strerror(errno));
    }
    // 服务器启动回调函数
    if (serv->onStart != NULL) {
        serv->onStart(serv);
    }
    //注册主线程reactor 检测是否有新链接的到来
    if (main_reactor->add(main_reactor, serv->serv_sock, TSW_EVENT_READ, tswServer_master_onAccept) < 0) {
        tswWarn("%s", "reactor add error");
        return TSW_ERR;
    }
    //主线程事件循环 等待客户端连接
    while (1) {
        int nfds;
        // 获取就绪事件
        nfds = main_reactor->wait(main_reactor);
        if (nfds < 0) {
            tswWarn("%s", "master thread epoll wait error");
            return TSW_ERR;
        }
        // 获取就绪事件触发回调函数 也就是查看是否有连接的到来 有连接触发 tswServer_master_onAccept 回调函数

        for (i = 0; i < nfds; i++) {
            tswReactorThread *tsw_reactor_thread;
            tswReactorEpoll *reactor_epoll_object = main_reactor->object;

            tswEvent *tswev = (tswEvent *)reactor_epoll_object->events[i].data.ptr;
            tswDebug("%s", "master thread handler the event");
            if (tswev->event_handler(main_reactor, tswev) < 0) {
                tswWarn("%s", "event_handler error");
                continue;
            }
        }
    }

    close(serv->serv_sock);

    return TSW_OK;
}
//服务器启动 主要做初始化的工作 申请内存和管道的
// 的创建
int tswServer_start(tswServer *serv)
{
    tswProcessPool *pool;
    tswPipe *pipe; 
    int i,j;
    //触发主线程回调函数
    serv->onMasterStart();
    //申请内存
    pool = (tswProcessPool *)malloc(sizeof(tswProcessPool));
    if (pool == NULL) {
        tswWarn("%s", "malloc error");
        return TSW_ERR;
    }
    //进程初始化 申请内存空间
    if (tswProcessPool_create(pool, serv->worker_num) < 0) {
        tswWarn("%s", "tswProcessPool_create error");
        return TSW_ERR;
    }
    //创建管道 并且赋值管道的属性
    for ( i = 0; i < serv->worker_num; i++) {
        tswPipeUnsock *object;
        //知识忙点
        pipe = &pool->pipes[i];  //申请的内存空间可以当成一个边长数组处理
        //创建socket pipe  管道
        if (tswPipeUnsock_create(pipe) < 0) {
            tswWarn("%s", "tswPipeUnsock_create error");
            return TSW_ERR;
        }
        //主进程写入数据 也就是 reactor现成的使用
        pool->workers[i].pipe_master = pipe->getFd(pipe, TSW_PIPE_MASTER);
        //工作进程写入数据 worker进程使用
        pool->workers[i].pipe_worker = pipe->getFd(pipe, TSW_PIPE_WORKER);
        pool->workers[i].pipe_object = pipe;
    }
   
    //创建工作进程 socketpair 在之前创建的 所以父子进程都可以使用
    for (j = 0; j < serv->worker_num; j++) {
        //创建worker 进程
        if (tswServer_create_worker(serv, pool, j) < 0) {
            tswWarn("%s", "tswServer_create_worker error");
            return TSW_ERR;
        }
    }

    tswProcessPool_info(pool);
    serv->process_pool = pool;
    //创建服务器代理
    if (tswServer_start_proxy(serv) < 0) {
        tswWarn("%s", "tswServer_start_proxy error");
        return TSW_ERR;
    }

    return TSW_OK;
}

/*
 //主线程接受客户端连接
  当有连接到来的时候 接受连接 此时的连接转态是 established
  从内核全连接队列中取出一个fd 进程数据交互和通讯
  fd是 和客户端通信的依据
 * reactor: Used to manage handle in tswEvent
*/
int tswServer_master_onAccept(tswReactor *reactor, tswEvent *tswev)
{
    int connfd;
    socklen_t len;
    struct sockaddr_in cliaddr;    //获取客户端的信息结构体
    tswServer *serv = reactor->ptr;
    tswReactor *sub_reactor;

    len = sizeof(cliaddr);
    //接受客户端连接
    connfd = accept(tswev->fd, (struct sockaddr *)&cliaddr, &len);
    if (connfd < 0) {
        tswWarn("%s", "accept error");
        return TSW_ERR;
    }
    //有连接的建立
   // printf("connect coming \n");
    serv->status->accept_count++;
    // 取模获取到对应reactor线程 客户端的数据由那个 reactor线程处理连接 和读写数据
    sub_reactor = &(serv->reactor_threads[connfd % serv->reactor_num].reactor);

    serv->connection_list[connfd].connfd = connfd;
    serv->connection_list[connfd].session_id = serv->status->accept_count;
    serv->connection_list[connfd].from_reactor_id = sub_reactor->id;
    serv->connection_list[connfd].serv_sock = serv->serv_sock;

    serv->session_list[serv->status->accept_count].session_id = serv->status->accept_count;
    serv->session_list[serv->status->accept_count].connfd = connfd;
    serv->session_list[serv->status->accept_count].reactor_id = sub_reactor->id;
    serv->session_list[serv->status->accept_count].serv_sock = serv->serv_sock;
    // 触发回调函数当有连接的时候 php 设置的回调函数
    serv->onConnect(serv->status->accept_count);
    // 注册对应的rteactor 线程 接受客户端的数据 是否可读
    if (sub_reactor->add(sub_reactor, connfd, TSW_EVENT_READ, tswServer_reactor_onReceive) < 0) {
        tswWarn("%s", "reactor add error");
        return TSW_ERR;
    }

    return TSW_OK;
}
//reactor线程接受客户端发送过来的数据
int tswServer_reactor_onReceive(tswReactor *reactor, tswEvent *tswev)
{
    int n;
    int pipe_master;
    char buffer[MAX_BUF_SIZE];
    tswReactorEpoll *reactor_epoll_object = reactor->object;
    tswEventData event_data;
    int worker_id;
    // 读取客户端的数据
    n = recv(tswev->fd, event_data.data, TSW_BUFFER_SIZE, 0);
    // 客户端关闭
    if (n == 0) {
        reactor->del(reactor, tswev->fd);
        close(tswev->fd);
        free(tswev);
        reactor->event_num -= 1;
        return TSW_OK;
    }
    //客户端异常 < 0


    event_data.info.len = n;
    event_data.info.from_id = reactor->id;
    event_data.info.fd = TSwooleG.serv->connection_list[tswev->fd].session_id;
    worker_id = tswev->fd % TSwooleG.serv->process_pool->worker_num; //算出对应的id
    //把客户端的数据发送给 相应的worker 进程的管道
    if (tswReactorThread_sendToWorker(TSwooleG.serv, &event_data, worker_id) < 0) {
        tswWarn("%s", "tswReactorThread_sendToWorker error");
        return TSW_ERR;
    }

    return TSW_OK;
}
//主线程启动
void tswServer_master_onStart(void)
{
    tswDebug("%s", "master thread started successfully");
}
// reactor 线程启动
void tswServer_reactor_onStart(int reactor_id)
{
    tswDebug("reactor thread [%d] started successfully", reactor_id);
}
// reactor 线程启动
void tswServer_master_onClose(void)
{
    //tswDebug("reactor thread [%d] started successfully", reactor_id);
    printf("close is end\n");
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
    // worker 进程的数据附送给reatcor 再有reactor 发送给客户端
    tswWorker_sendToReactor(&event_data);
    return TSW_OK;
}
