#include "tinyswoole.h"
#include "server.h"
#include "log.h"
// 获取管道的fd
static int tswPipeUnsock_getFd(tswPipe *pipe, int type)
{
    tswPipeUnsock *this = pipe->object;
    return type == TSW_PIPE_WORKER ? this->socks[0] : this->socks[1];
}
//管道读
static int tswPipeUnsock_read(tswPipe *pipe, void *data, int length)
{
    return read(((tswPipeUnsock *) p->object)->socks[0], data, length);
}
// 管道写
static int tswPipeUnsock_write(tswPipe *pipe, void *data, int length)
{
    return write(((tswPipeUnsock *) p->object)->socks[1], data, length);
}
//管道的创建
int tswPipeUnsock_create(tswPipe *pipe)
{
    tswPipeUnsock *object;

    object = (tswPipeUnsock *)malloc(sizeof(tswPipeUnsock));
    if (object == NULL) {
        tswWarn("%s", "malloc error");
        return TSW_ERR;
    }

    if (socketpair(AF_LOCAL, SOCK_STREAM, 0, object->socks) < 0) {
        tswWarn("%s", "socketpair error");
        free(object);
        return TSW_ERR;
    }

    pipe->object = object;
    pipe->getFd = tswPipeUnsock_getFd;
    pipe->read = tswPipeUnsock_read;
    pipe->write = tswPipeUnsock_write;

    return TSW_OK;
}
