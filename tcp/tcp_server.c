#include <stdio.h>
#include <arpa/inet.h>//inet_addr() sockaddr_in
#include <string.h>//bzero()
#include <sys/socket.h>//socket
#include <unistd.h>
#include <stdlib.h>//exit()

#define BUFFER_SIZE 1024   

int main() {

    char listen_addr_str[] = "0.0.0.0";

    size_t listen_addr = inet_addr(listen_addr_str);  // 转换成十进制的ip地址

    int port = 8080;

    int server_socket, client_socket;
    //创建服务端和客户端的 sockaddr_in
    struct sockaddr_in server_addr, client_addr; 

    socklen_t addr_size;

    char buffer[BUFFER_SIZE];//定义缓冲区大小

    int str_length;

    server_socket = socket(PF_INET, SOCK_STREAM, 0);  //创建soceket 地址 Ipv4 服务器类型 （SOCK_STREAM字节流）
    //置字节字符串s的前n个字节为零，无返回值
    bzero(&server_addr, sizeof(server_addr));  //置字节字符串s的前n个字节为零。
    /**********************绑定server 的状态 start************************/
    server_addr.sin_family = INADDR_ANY; // 这里可以理解为协议类型 设置地址家族

    server_addr.sin_port = htons(port);  //对应的端口

    server_addr.sin_addr.s_addr = listen_addr; //监听的地址
    /**********************绑定server 的状态 end************************/
    //绑定socket 绑定对应的ip和端口号
    if (bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) 
    {
        printf("bind error\n");
        return -1;
    }
    //监听socket 此时给半连接对列和全连接队列申请内存控制
    if (listen(server_socket, 5) == -1) 
    {
        printf("listen error\n");
        return -2;
    }

    printf("单进程 tcp 服务器创建成功\n"); //提示服务器创建成功

    addr_size = sizeof(client_addr); 
    //从全连接队列中取出一个 fd连接 进行通讯  对应的是客户端的信息 监听的是 server_socket
    client_socket = accept(server_socket, (struct sockaddr *) &client_addr, &addr_size);

    printf("client fd is %d connect success\n", client_socket);
    // 监听的socket 和已经完成连接的socket
    // 连接建立成功双方都可以读写数据 就像 对文件的操作一样
    char msg[] = "欢迎你连接成功\n";
    char bye[] = "服务端已经关闭\n";
    write(client_socket, msg, sizeof(msg));  //给当前客户端 进行数据通讯

    //阻塞在这等待服务器连接 不断的接受客户端的发过来的数据
    while (1) 
    {
        str_length = read(client_socket, buffer, BUFFER_SIZE); // 读取客户端缓存区的数据
        //没有数据的时候关闭客户端
        if (str_length == 0)    
        {
            write(client_socket,bye,str_length);//返回给客户端
            close(client_socket);  //关闭客户端
            printf("客户端关闭 fd 是:%d \n", client_socket);
            break;  //服务端 关闭一个 一个服务端只能接受一个客户端的连接 其他连接只能进行等待
            //continue;
        } else if(str_length > 0) {

            printf("client send string is:%s\n",buffer); 
            write(client_socket, buffer, str_length);//数据发送给客户端数据
        }else{ 
            //客户端异常
             close(client_socket);  //关闭客户端
            printf("read data is error\n");
            break;
        }

    }
    return 0;

}
