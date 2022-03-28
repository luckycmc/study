#ifndef TSW_SERVER_H_
#define TSW_SERVER_H_

#include "tinyswoole.h"

#define LISTENQ 10
#define MAX_BUF_SIZE 1024

tswServerG TSwooleG;
tswWorkerG TSwooleWG;
/** 
 * server 结构体
 * */
struct _tswServer {
    int serv_sock;

    void (*onStart)(tswServer *serv);  // 服务器启动
    void (*onConnect)(int fd);        //有连接到来
    void (*onReceive)(tswServer *serv, tswEventData *event_data); // 接受数据
    void (*onClose)(void); //服务端关闭

    void (*onMasterStart)(void);  //主进程启动

    int reactor_num;  // reactor 线程数
    tswReactorThread *reactor_threads;
    void (*onReactorStart)(int reactor_id);  //线程启动

    int worker_num; //工作进程的数量
    tswProcessPool *process_pool;  // 进程池

    tswConnection *connection_list; //连接
    tswSession *session_list;
    tswServerStatus *status;  //服务器的状态
    int reactor_pipe_num; //管道的个数
};

/*
 * Used for the reactor thread instead of the master thread
*/
struct _tswReactorThread {
    pthread_t thread_id;
    tswReactor reactor;
};

tswServer *tswServer_new(void);
int tswServer_start(tswServer *serv);
int tswServer_master_loop(tswServer *serv, int listenfd);
int tswServer_master_onAccept(tswReactor *reactor, tswEvent *tswev);
int tswServer_reactor_onReceive(tswReactor *reactor, tswEvent *tswev);
void tswServer_master_onStart(void);
void tswServer_reactor_onStart(int reactor_id);

int tswReactorThread_create(tswServer *serv);
int tswReactorThread_start(tswServer *serv);
int tswReactorThread_sendToWorker(tswServer *serv, tswEventData *event_data, int worker_id);
int tswReactorThread_onPipeReceive(tswReactor *reactor, tswEvent *tswev);

int tswServer_tcp_send(tswServer *serv, int fd, const void *data, size_t length);

enum tswWorkerPipeType {
    TSW_PIPE_WORKER     = 0,
    TSW_PIPE_MASTER     = 1,
};

#endif /* TSW_SERVER_H_ */
