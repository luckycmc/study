#include <stdio.h>
#include <arpa/inet.h>//inet_addr() sockaddr_in
#include <string.h>//bzero()
#include <sys/socket.h>//socket
#include <unistd.h>
#include <stdlib.h>//exit()
#include<sys/wait.h>//waitpid();
#include <ctype.h>

#define BUFFER_SIZE 1024
 
int main() 
{

    char listen_addr_str[] = "0.0.0.0";

    size_t listen_addr = inet_addr(listen_addr_str);

    int port = 8080;

    int server_socket, client_socket;

    struct sockaddr_in server_addr, client_addr;

    socklen_t addr_size;

    char buffer[BUFFER_SIZE];//缓冲区大小  字符串缓冲区的大小

    int str_length;

    pid_t pid;   // pid_t类型的变量pid

    int status = 0;//初始化状态

    server_socket = socket(PF_INET, SOCK_STREAM, 0);//创建套接字

    bzero(&server_addr, sizeof(server_addr));//初始化

    server_addr.sin_family = INADDR_ANY; //协议类型
    server_addr.sin_port = htons(port);

    server_addr.sin_addr.s_addr = listen_addr;

    //绑定sockect
    if (bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {

        printf("绑定失败\n");
        return -1;
    }
    //监听socket
    if (listen(server_socket, 5) == -1) {

        printf("监听失败\n");
        return -2;
    }

    printf("创建tcp多进程 服务器成功......\n");

    while (1) 
    {

        addr_size = sizeof(client_addr);
        //主进程负责接受客户端连接
        client_socket = accept(server_socket, (struct sockaddr *) &client_addr, &addr_size);

        printf("%d 连接成功\n", client_socket);
        printf("发送exit关闭\n");
        char msg[] = "恭喜你连接成功\n";

        write(client_socket, msg, sizeof(msg));

        pid = fork();   //创建子进程
        //父进程管理子进程的状态
        if (pid > 0) {

            sleep(1);//父进程,进行下次循环,读取客户端连接事件
            //回收子进程的状态
            waitpid(-1, &status, WNOHANG | WUNTRACED | WCONTINUED);

            if (WIFEXITED(status)) {

                printf("status = %d\n", WEXITSTATUS(status));
            }

            if (WIFSIGNALED(status)) { //如果子进程是被信号结束了 ，则为真

                printf("signal status = %d\n", WTERMSIG(status));

            }

            if (WIFSTOPPED(status)) {

                printf("stop sig num = %d\n", WSTOPSIG(status));

            }
            
            if (WIFCONTINUED(status)) {

                printf("continue......\n");

            }
            //子进程接受和处理业务逻辑
        } else if (pid == 0) {//子进程,进行阻塞式收发客户端数据，每一个请求创建一个进程 用于处理客户端的请求
         
            while (1)  //循环接受客户端的数据请求
            {
               /* memset是计算机中C/C++语言初始化函数。
               作用是将某一块内存中的内容全部设置为指定的值，
                这个函数通常为新申请的内存做初始化工作。
                */
                memset(buffer, 0, sizeof(buffer));

                str_length = read(client_socket, buffer, BUFFER_SIZE);  //  当前进程读取客户端内容

                if (str_length == 0)    //读取数据完毕关闭套接字
                {

                    close(client_socket);
                    printf("连接已经关闭: %d \n", client_socket);
                    exit(1);
                } else {

                    printf("%d 客户端发送数据:%s \n", client_socket, buffer);
                    if (buffer == "exit")
                    {   
                        printf("close current fd \n");
                        close(client_socket);
                    }
                    
                    printf("当前进程为%d\n", getpid());
                    write(client_socket, buffer, str_length);//发送数据给客户端
                }
            }

            break;  //跳出当前进程的循环，如果有新的连接进来则 启动新的进程

        } else {

            printf("创建子进程失败\n");
            exit(1);
        }
    }

    return 0;
}
