#include <stdio.h> 
#include <string.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <error.h> 
#include <errno.h> 
#include <sys/socket.h> 
#include <stdlib.h> 

const char *str= "SOCKET PAIR TEST.";

int main()
{
    char buf[128]={0};
    int socket_pair[2]; //定一个数组
    int ret;
    //创建管道
    ret = socketpair(AF_UNIX, SOCK_STREAM, 0, socket_pair);
    //创建失败返回-1
    if(ret == -1){
       printf("Error, socketpair create failed, errno(%d): %s\n", errno, strerror(errno));
       return -1;
    }
    //写数据
    int size = write(socket_pair[1],str,strlen(str));
    sleep(5);
    // 把数据读出来
    read(socket_pair[0],buf,size);
    printf("read data is: %s\n",buf);
    return 1;
}