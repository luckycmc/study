#include "tinyswoole.h"
#include "server.h"
#include "log.h"

int tswReactor_create(tswReactor *reactor, int max_event_num)
{
    if (tswReactorEpoll_create(reactor, max_event_num) < 0) {
        tswWarn("tswReactorEpoll_create error.");
        return TSW_ERR;
    }
    reactor->ptr = TSwooleG.serv;
    reactor->setHandler = tswReactor_setHandler;
    reactor->id = 0;
    return TSW_OK;
}
//设置回调函数句柄
int tswReactor_setHandler(tswEvent *tswev, int (*tswReactor_handler)(tswReactor *reactor, tswEvent *tswev))
{
    tswev->event_handler = tswReactor_handler; //事件对应的回调函数
    return TSW_OK;
}