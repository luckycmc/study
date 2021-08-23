#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <sys/errno.h>
#include <sys/socket.h>

#define NUMBER 10
#define LISTENMAX 20
#define IP "127.0.0.1"
#define PORT 80000
#define LINET 10000

static unsigned int threadParameter[NUMBER][8];//线程参数
pthread_t threadId[NUMBER];//线程id
pthread_mutex_t threadLock[NUMBER];//线程锁
pthread_cond_t count_nonzero[NUMBER];
int count1[NUMBER]={0};

static struct  dataPacket
{
       struct epoll_event ev;
       struct epoll_event waitEvent[LINET];
       int sockNumber[LINET]={0};
       int MAX=0;
       int epfd=0;
}ThreaDataPackage;
//减锁
void decrement_count (int i)
{
 
    pthread_mutex_lock (threadLock+i);
    while(count1[i]==0)
                pthread_cond_wait( count_nonzero+i, threadLock+i);
    count1[i]=0;
    pthread_mutex_unlock (threadLock+i);
}
//枷锁
void increment_count(int i)
{
    pthread_mutex_lock(threadLock+i);
    pthread_cond_signal(count_nonzero+i);
    count1[i]=1;
    pthread_mutex_unlock(threadLock+i);
}
/**
 * @brief 
 *   server socket
 * @param parameter 
 * @return void* 
 */
void * serverSocket(unsigned int *parameter)//线程主函数
{   char buf[1024];
    char buff[1024];
    pthread_detach(pthread_self());
    while(1)
    {
        decrement_count (parameter[7]);
        printf("启动线程：%d\n",parameter[7]);
        memset(buf,0,sizeof(buf));
        memset(buff,0,sizeof(buff));
        int len = recv(parameter[1], buf, 1024, MSG_NOSIGNAL);//非阻塞模式的消息接收
        //正常获取的数据
        if(len>0)
        {
            printf("%s\n",buf);
        }
        if(len==0)
        {
                for(int i=0;i<LINET;i++)
                {
                    if(parameter[1]==ThreaDataPackage.sockNumber[i])
                    {   ThreaDataPackage.MAX--;
                        ThreaDataPackage.sockNumber[i]=0;
                        close(ThreaDataPackage.sockNumber[i]);
                        printf("客户端%d下线\n",ThreaDataPackage.MAX);
                        if (epoll_ctl(ThreaDataPackage.epfd, EPOLL_CTL_DEL,parameter[1], &ThreaDataPackage.ev) < 0)//加入epoll事件集合
                            {
                                perror("epoll_ctl error:");
 
                            }
                        break;
                    }
                }
        }
        sprintf(buff ,"你好客户端我是第%d您发送的是：",parameter[7]);
        strcat(buff,buf);
        //发送数据
        len=send(parameter[1],buff,1024,MSG_NOSIGNAL);//非阻塞模式的消息发送
        memset(buff,0,sizeof(buff));
        parameter[0]= 0;//设置线程占用标志为"空闲"
    }
}
//线程池的创建
static int initThreadPool(void)//初始化数据
{   
    int i;
    int ret = 0;

    for(i=0;i<NUMBER;i++)
    {
        threadParameter[i][0]=0;
        threadParameter[i][7]=i;
        pthread_cond_init(count_nonzero+i,NULL); //条件变量的初始化
        pthread_mutex_init(threadLock+i,NULL);   // 锁的初始化
        //创建对应的线程
        a= pthread_create( threadId+ i, NULL, (void* (*)(void *))serverSocket,(void *)(threadParameter[i]));
        if(ret!=0)
        {
            perror("pthread_create error:");
            return -1;
        }
    }
    return 0;
}
//初始化监听
static int  initListen(char*ip,int port,int listenMax)//初始化监听
{  
    int a=0;
    //socket 
    int sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0)
    {
        perror("sockt error:");
        close(sockfd);
        return -1;
    }
    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family=AF_INET;
    inet_pton(AF_INET,ip,&(server_addr.sin_addr));
    server_addr.sin_port=htons(port);
    int opt = 1;
    //设置为非阻塞
    setsockopt(sockfd, SOL_SOCKET,SO_REUSEADDR, (const void *) &opt, sizeof(opt));
    //绑定
    a  = bind(sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr));
    if(a<0)
    {
        perror("bind error:");
        close(sockfd);
        return -1;
    }
    //监听
    a = listen(sockfd,listenMax);
    if(a<0)
    {
        perror("listen error:");
        close(sockfd);
        return -1;
    }
    return sockfd;  //返回对应的socket
}
//文件描述符设置为非阻塞的
bool setNonBlock(int fd)//设置文件描述符为NonBlock
{
   int flags = fcntl(fd, F_GETFL, 0);
   flags |= O_NONBLOCK;
   if(-1 == fcntl(fd, F_SETFL, flags))
   {
       return false;
   }
    return true;
}
//数函数执行
int main()
{
    int acceptSockfd=0;//accept返回的套接字
    int sockfd=0;//服务器套接字
    int nfds=0;//触发事件的个数
    socklen_t addrLen; //地址信息长度
    struct sockaddr_in clinetAddr; //IPv4地址结构
    if(0!=initThreadPool())
    {
        perror("initThreadPool error:");
        exit(-1);
    }
    sockfd=initListen(IP,PORT,LISTENMAX);
    ThreaDataPackage.sockNumber[0]=sockfd;
    if(sockfd<0)
    {
        perror("initListen error:");
        exit(-1);
    }
    ThreaDataPackage.epfd = epoll_create(8);//生成文件描述符
    ThreaDataPackage.ev.events = EPOLLIN | EPOLLET;//对应的文件描述符可读并且是et的epoll工作模式
    ThreaDataPackage.ev.data.fd =sockfd ;
    if (epoll_ctl(ThreaDataPackage.epfd , EPOLL_CTL_ADD,sockfd, &ThreaDataPackage.ev) < 0)//加入epoll事件集合
    {
            perror("epoll_ctl error:");
            exit(-1);
    }
    //epoll 进入事件循环等待
    while(1)
    {
        nfds = epoll_wait(ThreaDataPackage.epfd , ThreaDataPackage.waitEvent, ThreaDataPackage.MAX+1, -1);
        printf("nfds::%d\n",nfds);
        for(int i=0;i<nfds;i++)
         {
          if((sockfd==ThreaDataPackage.waitEvent[i].data.fd)&&(EPOLLIN==ThreaDataPackage.waitEvent[i].events&EPOLLIN))
            {
                 addrLen=sizeof(struct sockaddr_in);
                 bzero(&clinetAddr,addrLen);
                 for(int j=0;j<LINET;j++)
                 {
                     if(ThreaDataPackage.sockNumber[j]==0)
                     {
                         ThreaDataPackage.sockNumber[j]= accept(sockfd, (struct sockaddr *)&clinetAddr, &addrLen);
                         if(ThreaDataPackage.sockNumber[j]<0)
                         {
                             perror("accept error:");
                             continue;
                         }
                         else
                         {
                            ThreaDataPackage.ev.data.fd = ThreaDataPackage.sockNumber[j];
                            ThreaDataPackage.ev.events = EPOLLIN|EPOLLET;
                            if (epoll_ctl(ThreaDataPackage.epfd , EPOLL_CTL_ADD,ThreaDataPackage.sockNumber[j], &ThreaDataPackage.ev) < 0)//加入epoll事件集合
                                {
                                    perror("epoll_ctl error:");
                                    exit(-1);
                                }
                             setNonBlock(ThreaDataPackage.sockNumber[j]);//设置为非阻塞
                             ThreaDataPackage.MAX++;
                             printf("客户端%d上线\n",ThreaDataPackage.MAX);
                             break;
                         }
                     }
                 }
            }
           else if(ThreaDataPackage.waitEvent[i].data.fd>3&&( EPOLLIN == ThreaDataPackage.waitEvent[i].events & (EPOLLIN|EPOLLERR)))
            {
                for(int j=0;j<NUMBER;j++)
                  {
                     if(0==threadParameter[j][0])
                       {
                        threadParameter[j][0]=1;//设置活动标志为"活动"
                        threadParameter[j][1]=ThreaDataPackage.waitEvent[i].data.fd;//客户端的套接字
                        increment_count(j);
                        break;
                      }
                   }
             }
         }
    }
    return 0;
}
