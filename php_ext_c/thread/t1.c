#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
//全局数据 
struct SockInfo
{
    int fd;                       //通讯的fd
    pthread_t tid;                //线程id
    struct sockaddr_in addr;      // 地址信息
}

struct SockInfo infos[128];
//线程工作的入口地址
void* working(void *arg)
{   
    
    return NULL;
}

int main()
{
    
     // 1.创建监听套接字
     int fd = socket(AF_INET,SOCK_STREAM,0);
     if(fd == -1){
         perror("socket create is error");
         exit(0);
     }

     //2.绑定数据信息
     struct sockaddr_in add;
     addr.sin_family = AF_INET; // ipv4
     addr.sin_port = htons(8080); //// 字节序应该是网络字节序
     add.sin_addr.sin_addr = INADDR_ANY;// // == 0, 获取IP的操作交给了内核
     ret = bind(fd,(struct sockaddr *)&addr,sizeof(addr));
     if(ret == -1){
          perror("bind");
          exit(0);
     }
     //3.设置监听
     ret = listen(fd,100);
     if(ret == -1){
          perror("listen\n");
          exit(0);
     }

     //数据初始化
     // 4. 等待, 接受连接请求
    int len = sizeof(struct sockaddr);

    // 数据初始化
    int max = sizeof(infos) / sizeof(infos[0]);
    for(int i=0; i<max; ++i)
    {
        bzero(&infos[i], sizeof(infos[i]));
        infos[i].fd = -1;
        infos[i].tid = -1;
    }
    // // 主线程监听, 子线程通信
    while(1)
    {
       // 创建子线程
        struct SockInfo* pinfo;
        for(int i=0; i<max; ++i)
        {
            if(infos[i].fd == -1)
            {
                pinfo = &infos[i];
                break;
            }
            if(i == max-1)
            {
                sleep(1);
                i--;
            }
        }
        //接受客户端连接
        int connfd = accept(fd, (struct sockaddr*)&pinfo->addr, &len);
        printf("parent thread, connfd: %d\n", connfd);
        if(connfd == -1)
        {
            perror("accept");
            exit(0);
        }
        pinfo->fd = connfd; //当前的fd
        pthread_create(&pinfo->tid, NULL, working, pinfo);
        pthread_detach(pinfo->tid);
    }
    // 释放资源
    close(fd);  // 监听
    return 1;
}