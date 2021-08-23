#include <stdio.h>

#include <arpa/inet.h>//inet_addr() sockaddr_in

#include <string.h>//bzero()

#include <sys/socket.h>//socket

#include <unistd.h>

#include <stdlib.h>//exit()

#define BUFFER_SIZE 1024   //定义缓冲区的大小

 

int main() {

    char listen_addr_str[] = "0.0.0.0";

    size_t listen_addr = inet_addr(listen_addr_str);

    int port = 8080;

    int server_socket, client_socket;

    struct sockaddr_in server_addr, client_addr;

    socklen_t addr_size;

    char buffer[BUFFER_SIZE];//缓冲区大小

    int str_length;

    server_socket = socket(PF_INET, SOCK_STREAM, 0);//创建套接字


    bzero(&server_addr, sizeof(server_addr));//初始化

    server_addr.sin_family = INADDR_ANY;

    server_addr.sin_port = htons(port);

    server_addr.sin_addr.s_addr = listen_addr;

 
    //绑定
    if (bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {

         printf("绑定失败\n");
         return -1;
    }
    // 监听
    if (listen(server_socket, 5) == -1) {

        printf("监听失败\n");
        return -2;
    }

    printf("创建select tcp服务器成功\n");
    // fd_set  是select 的一种数据结构
    fd_set reads,copy_reads;
    /*
       FD_ZERO(fd_set *fdset);将指定的文件描述符集清空，在对文件描述符集合进行设置前，必须对其进行初始化，如果不清空，由于在系统分配内存空间后，通常并不作清空处理，所以结果是不可知的。
       FD_SET(fd_set *fdset);用于在文件描述符集合中增加一个新的文件描述符。 
       FD_CLR(fd_set *fdset);用于在文件描述符集合中删除一个文件描述符。 
       FD_ISSET(int fd,fd_set *fdset);用于测试指定的文件描述符是否在该集合中。
    */
    int fd_max,fd_num;

    struct timeval timeout;

    FD_ZERO(&reads);//初始化清空socket集合

    FD_SET(server_socket,&reads);  //用于在文件描述符集合中增加一个新的文件描述符。

    fd_max = server_socket;
    // 监听客户端的请求
    while (1) {

        copy_reads = reads;

        timeout.tv_sec = 5;

        timeout.tv_usec = 5000;

        //无限循环调用select 监视可读事件
        /**
         * 1:客户端的socket标识符是存在一个fd_set类型中的集合中的,客户端大小由fd_set大小决定,开发时需要考虑到这个的最大值
           2:每次调用select函数之前,都得将集合重新传给select,效率较慢;
           3:每次调用完select函数,就算返回1,也会将集合全部遍历一遍,效率较慢
         */
        if((fd_num = select(fd_max+1, &copy_reads, 0, 0, &timeout)) == -1) {

            perror("select error");
            break;
        }

        if (fd_num == 0){//没有变动的socket  当前循环退出
            continue;
        }
        
        int i;  //数据循环

        for(i=0;i<fd_max+1;i++)
        {

            if(FD_ISSET(i,&copy_reads)){

                if (i==server_socket) //server_socket变动,代表有新客户端连接
                {
                    addr_size = sizeof(client_addr);

                    client_socket = accept(server_socket, (struct sockaddr *) &client_addr, &addr_size);

                    printf("%d 连接成功\n", client_socket);

                    char msg[] = "恭喜你连接成功";

                    write(client_socket, msg, sizeof(msg));

                    FD_SET(client_socket,&reads);

                    if(fd_max < client_socket){

                        fd_max=client_socket;

                    }

                }else{

                    memset(buffer, 0, sizeof(buffer));

                    str_length = read(i, buffer, BUFFER_SIZE);

                    if (str_length == 0)    //读取数据完毕关闭套接字
                    {

                        close(i);

                        printf("连接已经关闭: %d \n", i);

                        FD_CLR(i, &reads);//从reads中删除相关信息

                    } else {

                        printf("%d 客户端发送数据:%s \n", i, buffer);

                        write(i, buffer, str_length);//将数据发送回客户端

                    }

                }
            }

        }
    }

    return 0;

}
