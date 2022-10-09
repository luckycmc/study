#include "tinyswoole.h"
#include "socket.h"
#include "log.h"
// socket 的创建
int tswSocket_create(int type)
{
    int _domain;
    int _type;

    if (type == TSW_SOCK_TCP) {
        _domain = AF_INET;
        _type = SOCK_STREAM;
    } else if (type == TSW_SOCK_UDP) {
        _domain = AF_INET;
        _type = SOCK_DGRAM;
    } else {
        tswWarn("unknown socket type [%d]", type);
        return TSW_ERR;
    }

    return socket(_domain, _type, 0);
}
// socket的绑定
int tswSocket_bind(int sock, int type, char *host, int port)
{
    int ret;
    struct sockaddr_in servaddr;

    if (type == TSW_SOCK_TCP) {
        bzero(&servaddr, sizeof(servaddr));
        inet_aton(host, &(servaddr.sin_addr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(port);
        ret = bind(sock, (struct sockaddr *)&servaddr, sizeof(servaddr));
        if (ret < 0) {
            tswWarn("%s", strerror(errno));
            return TSW_ERR; 
        }
    } else {
        return TSW_ERR;
    }

    return ret;
}

// socket 设置为非阻塞
int tswSocket_set_nonblock(int sock)
{
    int flags;

    flags = fcntl(sock, F_GETFL, 0);
    if (flags < 0) {
        stWarn("Error has occurred: (errno %d) %s", errno, strerror(errno));
        return -1;
    }
    flags = fcntl(sock, F_SETFL, flags | O_NONBLOCK);
    if (flags < 0) {
        stWarn("Error has occurred: (errno %d) %s", errno, strerror(errno));
        return -1;
    }
    return 0;
}

// recv

//send 也可以封装到这个地方

