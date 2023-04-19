#include <stdio.h>

#include <arpa/inet.h>//inet_addr() sockaddr_in

#include <string.h>//bzero()

#include <sys/socket.h>//socket

#include <unistd.h>

#include <stdlib.h>//exit()

#include <sys/epoll.h> //epoll


#define BUFFER_SIZE 1024

#define CLIENT_MAX_SIZE 1024

#define PORT 8080
/**************
epoll是Linux下多路复用IO接口select/poll的增强版本，它能显著提高程序在大量并发连接中只有少量活跃的情况下的系统CPU利用率，
 因为它会复用文件描述符集合来传递结果而不用迫使开发者

每次等待事件之前都必须重新准备要被侦听的文件描述符集合（用户态和内核态共享同一片文件描述符表内存），
另一点原因就是获取事件的时候，它无须遍历整个被侦听的描述符集，只要遍历那

些被内核IO事件异步唤醒而加入Ready队列的描述符集合就行了。

目前epell是linux大规模并发网络程序中的热门首选模型。

epoll除了提供select/poll那种IO事件的电平触发（Level Triggered）外，还提供了边沿触发（Edge Triggered），
这就使得用户空间程序有可能缓存IO状态，减少epoll_wait/epoll_pwait的调用，提高应用程序效率。

可以使用cat命令查看一个进程可以打开的socket描述符上限
 * */
int main() 
{

    char listen_addr_str[] = "0.0.0.0";
    
    //  size_t 的取值range是目标平台下最大可能的数组尺寸,
    size_t listen_addr = inet_addr(listen_addr_str);  //  inet_addr 功能是将一个点分十进制的IP转换成一个长整数型数

    int server_socket, client_socket;   //定义两个socket 

    struct sockaddr_in server_addr, client_addr;    //sockaddr_in 网络编程结构体 属于内核

    socklen_t addr_size;

    char buffer[BUFFER_SIZE];//缓冲区大小

    int str_length;

    server_socket = socket(PF_INET, SOCK_STREAM, 0);//创建套接字

    bzero(&server_addr, sizeof(server_addr));//初始化  // 将字符串s的前n个字节置为0，一般来说n通常取sizeof(s),将整块空间清零。
 
    server_addr.sin_family = INADDR_ANY;  // 协议族设置

    server_addr.sin_port = htons(PORT); //端口号

    server_addr.sin_addr.s_addr = listen_addr; // 监听地址

    if (bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {

        printf("绑定失败\n");
        return -1;
    }
    //网络进入半连接状态 产生listen_fd
    if (listen(server_socket, 5) == -1) {

        printf("监听失败\n");
        return -2;
    }

    printf("创建epoll tcp服务器成功\n");

    struct epoll_event event;//监听事件

    struct epoll_event wait_event_list[CLIENT_MAX_SIZE];//监听结果

    int fd[CLIENT_MAX_SIZE];

    int j = 0;

    int epoll_fd = epoll_create(10);//创建epoll句柄,里面的参数10没有意义

    if (epoll_fd == -1) {

        printf("创建epoll句柄失败\n");
        return -1;
    }

    event.events = EPOLLIN;//可读事件

    event.data.fd = server_socket;//server_socket
    //向内核申请一块创建epoll 的空间
    int result = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_socket, &event);

    if (result == -1) {

        printf("注册epoll 事件失败\n");
        return -1;
    }
    while (1)   //进入事件循环 
    {       
        
        //阻塞等待事件的发生  epoll_fd 对应的epoll_create  创建的 句柄
        //  wait_event_list 对应的事件   CLIENT_MAX_SIZE 每一次去多少个事件   
        result = epoll_wait(epoll_fd, wait_event_list, CLIENT_MAX_SIZE, -1);//阻塞
       //没有对应的事件发生,继续下一轮事件循环
        if (result <= 0) {
            continue;
        }
        printf("---event num is :%d\n",result); 
        //获取对应的可读可写事件
        //遍历对应的事件 其实经过内核处理后 已经发生的已经形成一个队列直接拿出来处理即可
        // 事件有 可读可写 可异常操作
        for (j = 0; j < result; j++)    
        {

            printf("%d ----触发事件----- %d \n", wait_event_list[j].data.fd, wait_event_list[j].events);

            //server_socket触发事件 连接成功的时候,当有客户端连接的时候
            if (server_socket == wait_event_list[j].data.fd && EPOLLIN == wait_event_list[j].events & EPOLLIN)
            {

                addr_size     = sizeof(client_addr);
                //接受客户端连接,客户端连接服务器端连接成功 ，此时进入全连接状态，去除对应的可操作的fd
                client_socket = accept(server_socket, (struct sockaddr *) &client_addr, &addr_size);

                printf("%d 连接成功\n", client_socket);  // server 端提示连接成功

                char msg[] = "恭喜你连接成功";

                write(client_socket, msg, sizeof(msg)); // 数据返回个客户端

                event.data.fd = client_socket;   // event 事件数据fd

                event.events = EPOLLIN;//可读或错误
                //客户端fd 加入到 对应的epoll的事件结构中去
                result = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_socket, &event);

                if (result == -1) {

                    printf("注册客户端 epoll 事件失败\n");
                    exit(1);
                }
                continue;  //  跳出当前的事件循环
            }
            //当客户端数据可读可写的时候处理对应的数据
            if ((wait_event_list[j].events & EPOLLIN)||(wait_event_list[j].events & EPOLLERR))//可读或发生错误
            {

                memset(&buffer, 0, sizeof(buffer));

                str_length = read(wait_event_list[j].data.fd, buffer, BUFFER_SIZE);

                if (str_length == 0)    //读取数据完毕关闭套接字
                {
                    close(wait_event_list[j].data.fd);

                    event.data.fd = wait_event_list[j].data.fd;
                    //删除对应的fd节点
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, wait_event_list[j].data.fd, &event);

                    printf("连接已经关闭: %d \n", wait_event_list[j].data.fd);

                } else {
                   
                    printf("客户端发送数据:%s \n", buffer); 
                    // 数据返回给客户端 fd ,buffer,str_length 长度 
                    // wait_event_list[j].data.fd 对应当前监听的fd
                    write(wait_event_list[j].data.fd, buffer, str_length);//执行回声服务  即echo
                }
            }
        }
    }

    return 0;
}