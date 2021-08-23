#include <pthread.h>
#include <sys/types.h>            /* See NOTES */
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>

static const size_t SOCKET_BUFFER_SIZE = 32 * 1024;

//线程函数
void *pthread_1(void *arg)
{
    int fd = *((int *)arg);  //强制转化为整型
    char buf[512];
    int len;
    int cnt;
    while (1)
    {
        len = sprintf(buf, "hello, main pthread, cnt = %d", cnt++);
        write(fd, buf, len);

        len = read(fd, buf, 500);
        buf[len] = '\0';
        printf("%s\n", buf);
        sleep(5);
    }
    return NULL;
}
//主函数
int main()
{
   int sockets[2];
    pthread_t thread_id;
    if (socketpair(AF_UNIX, SOCK_SEQPACKET, 0, sockets)) {
        printf("socketpair error\n");
        return -1;
    }
    int bufferSize = SOCKET_BUFFER_SIZE;
    /* 创建4个buff, sockets[0]的发送buff和接收buff; sockets[1]的发送buff和接收buff*/
    setsockopt(sockets[0], SOL_SOCKET, SO_SNDBUF, &bufferSize, sizeof(bufferSize));
    setsockopt(sockets[0], SOL_SOCKET, SO_RCVBUF, &bufferSize, sizeof(bufferSize));
    setsockopt(sockets[1], SOL_SOCKET, SO_SNDBUF, &bufferSize, sizeof(bufferSize));
    setsockopt(sockets[1], SOL_SOCKET, SO_RCVBUF, &bufferSize, sizeof(bufferSize));
    //创建线程
    int res = pthread_create(&thread_id, NULL, pthread_1, (void *)(&sockets[1]));
    if (res) {
        printf("pthread_create error\n");
        return -1;
    }
    int fd = sockets[0];
    char buf[512];
    int len;
    int cnt = 0;
    
    while (1) {
        len = sprintf(buf, "hello, pthread1, cnt = %d", cnt++);
        write(fd, buf, len);  //将buf中的内容通过fd句柄发送到snd buff

        len = read(fd, buf, 500);   //通过读fd中的rcv buff， 将内容读到buf中，然后打印出来
        buf[len] = '\0';
        printf("%s\n", buf);
        sleep(5);
    }
    return 0;
}