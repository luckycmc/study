#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>

#define PORT  8080

#define ISspace(x) isspace((int)(x))  
//服务器的信息
#define SERVER_STRING "Server: jdbhttpd/0.1.0\r\n"  
//设置返回数据的协议 表示为http 协议
#define MESSAGE "HTTP/1.1 200 OK\r\nContent-Length: 11\r\n\r\nhello world"
//声明子进程处理的函数
int childwork(int cfd);
//接受客户端请求的数据
void accept_request(int cfd,char *buf);
//解析 http 发送的数据 第一行
int get_line(char *allbuf, int idx,char *linebuf);
//404 请求
void bad_request(int cfd);
//打开资源文件 也就是对应的html文件
void cat(int client, FILE *resource);
//文件资源找不到
void not_found(int cfd);
//打开对应的文件信息
void serve_file(int client, const char *filename);
//信号处理函数
void callback(int num)
{
    while(1)
    {
          
            pid_t pid = waitpid(-1,NULL,WNOHANG);
            if(pid <= 0)
            {
                  printf("子进程正在运行, 或者子进程被回收完毕了\n");
                  break;
            }
             printf("child die, pid = %d\n", pid);
    }
}

//主函数处理
int main()
{
    
    //1.创建监听套接字
    int lfd = socket(AF_INET,SOCK_STREAM,0); //创建soceket 地址 Ipv6 服务器类型
    if(lfd == -1){
        
          perror("socket create is error\n");
          exit(0);
    }
    // 2. 将socket()返回值和本地的IP端口绑定到一起
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    // INADDR_ANY代表本机的所有IP, 假设有三个网卡就有三个IP地址
    // 这个宏可以代表任意一个IP地址
    // 这个宏一般用于本地的绑定操作
    addr.sin_addr.s_addr = INADDR_ANY;
    //3.绑定服务端
    int ret = bind(lfd,(struct sockaddr *)&addr,sizeof(addr));
    if(ret == -1){
         
          perror("bind is error\n");
          exit(0);
    }
    //4.设置监听
    ret = listen(lfd,128);
    if(ret == -1){
         perror("listen is error\n");
         exit(1);
    }

   // 注册信号的捕捉
    struct sigaction act;
    act.sa_flags = 0;
    act.sa_handler = callback;  //信号的回调函数
    sigemptyset(&act.sa_mask);
    sigaction(SIGCHLD, &act, NULL); // 函数绑定
    /***注册捕获信号 end***/
    // 5.接受客户端连接
    while(1)
    {
          //获取客户端的信息 结构体
          struct sockaddr_in client_addr;
          int client_addr_size = sizeof(client_addr);
          //如果没有客户端过来则 会阻塞进程
          int cfd = accept(lfd,(struct sockaddr *)&client_addr,&client_addr_size);
          if(cfd == -1){
                if(errno == EINTR)
                {
                    // accept调用被信号中断了, 解除阻塞, 返回了-1
                    // 重新调用一次accept
                    continue;
                }
            perror("accept");
            exit(0);
          }  

          // 打印客户端的地址信息
         char ip[24] = {0};
         printf("客户端的IP地址: %s, 端口: %d\n",
               inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, ip, sizeof(ip)),
               ntohs(client_addr.sin_port));

        // 新的连接到来,创建子进程
        pid_t pid = fork();
        //子进程空间
        if(pid == 0){
            
             //子进程--> 和客户端通讯
            // 通讯的文件描述符cfd 被拷贝到子进程中
            // 子进程不负责监听客户端
            close(lfd);
            while(1)
            {
                int ret = childwork(cfd);
                if(ret < 0){
                    break;
                }
            }
            printf("current pid is %d\n",getpid());
            //退出子进程关闭cfd
            close(cfd);
            exit(0); //当前进程退出后会有相应的信号buhip
        }else if(pid > 0){ //父进程空间
          
            //不参与客户端通讯 关闭cfd
            close(cfd);
        }else{
            perror("create process is error\n");
            exit(0);
        }
    }
    return 0;
}

//子进程与客户端通讯 单独的进程通讯
int childwork(int cfd)
{
    //接受数据缓冲区
    char buf[1024];
    memset(buf, 0, sizeof(buf));
    //读取客户端的数据
    int len = read(cfd,buf,sizeof(buf));
    if(len > 0)
    {
          accept_request(cfd,buf);
    }
    else if(len  == 0)
    {
        printf("客户端断开了连接...\n");
        close(cfd);
       // exit(0); // 需要退出当前进程
       return -1;
    }else{
        perror("read cliennt data is error\n");
        close(cfd);
        //exit(0);// 需要退出当前进程
         return -1;
    }
    return len;
}
// 接受客户端的请求数据
void accept_request(int cfd,char *buf)
{
     //接受数据缓冲区
     char linebuf[1024]={0};
   // memset(buf, 0, sizeof(buf));
    //获取http中的GET  请求
     //读取客户端的数据
    //int len = read(cfd,buf,sizeof(buf));
    /**
       接受请求的http协议
        GET / HTTP/1.1
        User-Agent: curl/7.29.0
        Host: 127.0.0.1:8080
        //Accept: *//*

    **/
    int idx = get_line(buf,0,linebuf);
    if(idx == -1){
         
          printf("http content is error\n");
          exit(0);
    }
    //printf("%s\n",linebuf);
    //判断是否会get 请求
    if(strstr(linebuf,"GET")){
         //not_found(cfd);
         write(cfd,MESSAGE,sizeof(MESSAGE));
         //处理get 请求 读取文件
         // 文件不存在设置为404 
         //cgi 请求等相关信息的设置
         //解析对应的html文件
        // serve_file(cfd, './index.html');  

    }else{

    }
}


/****************************** 获取第一行的http数据 start****************************************/  
int get_line(char *allbuf, int idx,char *linebuf)  
{  
     int len = strlen(allbuf);
     for(; idx<len; idx++)
     {
           //遇到第一个\r\n 返回数据
           if(allbuf[idx]=='\r'&&allbuf[idx+1]=='\n' ){
                return idx+2;
           }else{
               *(linebuf++) = allbuf[idx];
           }

     }
     return -1;
}  

/****************************** 获取第一行的http数据 end****************************************/  
// 页面发生错误时的请求
void bad_request(int cfd)  
{  
    char buf[1024];  
  
    /*回应客户端错误的 HTTP 请求 */  
    sprintf(buf, "HTTP/1.0 400 BAD REQUEST\r\n");  
    send(cfd, buf, sizeof(buf), 0);  
    sprintf(buf, "Content-type: text/html\r\n");  
    send(cfd, buf, sizeof(buf), 0);  
    sprintf(buf, "\r\n");  
    send(cfd, buf, sizeof(buf), 0);  
    sprintf(buf, "<P>Your browser sent a bad request, ");  
    send(cfd, buf, sizeof(buf), 0);  
    sprintf(buf, "such as a POST without a Content-Length.\r\n");  
    send(cfd, buf, sizeof(buf), 0);  
}  
// 404 页面
void not_found(int cfd)  
{  
    char buf[1024];  
  
    /* 404 页面 */  
    sprintf(buf, "HTTP/1.0 404 NOT FOUND\r\n");  
    send(cfd, buf, strlen(buf), 0);  
    /*服务器信息*/  
    sprintf(buf, SERVER_STRING);  
    send(cfd, buf, strlen(buf), 0);  
    sprintf(buf, "Content-Type: text/html\r\n");  
    send(cfd, buf, strlen(buf), 0);  
    sprintf(buf, "\r\n");  
    send(cfd, buf, strlen(buf), 0);  
    sprintf(buf, "<HTML><TITLE>Not Found</TITLE>\r\n");  
    send(cfd, buf, strlen(buf), 0);  
    sprintf(buf, "<BODY><P>The server could not fulfill\r\n");  
    send(cfd, buf, strlen(buf), 0);  
    sprintf(buf, "your request because the resource specified\r\n");  
    send(cfd, buf, strlen(buf), 0);  
    sprintf(buf, "is unavailable or nonexistent.\r\n");  
    send(cfd, buf, strlen(buf), 0);  
    sprintf(buf, "</BODY></HTML>\r\n");  
    send(cfd, buf, strlen(buf), 0);  
}  

/*************************** 读取对应的文件 start *******************************************/  
void cat(int cfd, FILE *resource)  
{  
    char buf[1024];  
  
    /*读取文件中的所有数据写到 socket */  
    fgets(buf, sizeof(buf), resource);  
    while (!feof(resource))  
    {  
        send(cfd, buf, strlen(buf), 0);
    }  
}  
  
/**********************************************************************/  


/**********************************************************************/  
void serve_file(int client, const char *filename)  
{  
    FILE *resource = NULL;  
    int numchars = 1;  
    char buf[1024];  
  
    /*读取并丢弃 header */  
    buf[0] = 'A'; buf[1] = '\0';  
    while ((numchars > 0) && strcmp("\n", buf))  /* read & discard headers */  
        numchars = get_line(client, buf, sizeof(buf));  
  
    /*打开 sever 的文件*/  
    resource = fopen(filename, "r");  
    if (resource == NULL)  
        not_found(client);  
    else  
    {  
        /*写 HTTP header */  
        headers(client, filename);  
        /*复制文件*/  
        cat(client, resource);  
    }  
    fclose(resource);  
}  
  
/**********************************************************************/  


/**********************************************************************/  
//文件的头部
void headers(int client, const char *filename)  
{  
    char buf[1024];  
    (void)filename;  /* could use filename to determine file type */  
  
    /*正常的 HTTP header */  
    strcpy(buf, "HTTP/1.0 200 OK\r\n");  
    send(client, buf, strlen(buf), 0);  
    /*服务器信息*/  
    strcpy(buf, SERVER_STRING);  
    send(client, buf, strlen(buf), 0);  
    sprintf(buf, "Content-Type: text/html\r\n");  
    send(client, buf, strlen(buf), 0);  
    strcpy(buf, "\r\n");  
    send(client, buf, strlen(buf), 0);  
}  
  
/**********************************************************************/  