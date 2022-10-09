#include "tinyswoole.h"
#include "server.h"
#include "log.h"
#include "socket.h"
// 获取管道描描述符
static int tswPipeUnsock_getFd(tswPipe *pipe, int type)
{
    tswPipeUnsock *this = pipe->object; //获取读管道和写管道
    return type == TSW_PIPE_WORKER ? tswSocket_set_nonblock(this->socks[0]) : tswSocket_set_nonblock(this->socks[1]);
}

static int tswPipeUnsock_read(tswPipe *pipe, void *data, int length)
{
    
}

static int tswPipeUnsock_write(tswPipe *pipe, void *data, int length)
{
    
}
// 管道的创建 主要用于 进程通讯
int tswPipeUnsock_create(tswPipe *pipe)
{
    tswPipeUnsock *object; //数组fd

    object = (tswPipeUnsock *)malloc(sizeof(tswPipeUnsock));
    if (object == NULL) {
        tswWarn("%s", "malloc error");
        return TSW_ERR;
    }
    //创建socketpair 双向管道
    if (socketpair(AF_LOCAL, SOCK_STREAM, 0, object->socks) < 0) {
        tswWarn("%s", "socketpair error");
        free(object);
        return TSW_ERR;
    }

    pipe->object = object; //指向管道描述符fd
    pipe->getFd = tswPipeUnsock_getFd;  //获取管道的fd
    pipe->read = tswPipeUnsock_read;    //读取管道
    pipe->write = tswPipeUnsock_write;  // 写入管道
   
    return TSW_OK;
}
