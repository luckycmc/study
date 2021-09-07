#include "multi_thread.h"

//子线程处理对应的数据请求
void *rec_func(void *arg)
{
     int sockfd,new_fd,nbytes;
     char buffer[RECVBUFSIZE];
     printf("current thread is %lu\n",pthread_self());
     //参数转换成对应的整形
     new_fd = *((int *) arg); //转换成整形的数据
     //printf("children accept fd is %d\n",new_fd);
     free(arg); 
     // 接受客户端消息
     while(1)
     {
            //接受客户端的数据
            if((nbytes = recv(new_fd,buffer, RECVBUFSIZE,0))==-1)
            {
                fprintf(stderr,"Read Error:%s\n",strerror(errno));
                exit(1);
            }
            
            //客户端出错返回 -1
            if(nbytes == -1)
            {
                close(new_fd);
                break;  //跳出当前循环
            }
            //客户端关闭
            if(nbytes == 0)
            {
                 printf("client close fd is %d\n",new_fd);
                 close(new_fd);
                 break;
            }
            //给结尾添加表示符
            buffer[nbytes] = '\0';

            //打印接受的数据
            //printf("I have received:%s\n",buffer);

            //数据返回给客户端
            if(send(new_fd,buffer,strlen(buffer),0) == -1)
            {
                  fprintf(stderr,"Write Error:%s\n",strerror(errno));
                  exit(1);
            }
     }

    // return NULL;
}

//主线程函数
void master_thread()
{
     char buffer[RECVBUFSIZE];
     int sockfd,new_fd,nbytes;

     struct sockaddr_in server_addr;
     struct sockaddr_in client_addr;
     
     int portnumber = 8080; //端口号
     int sin_size;
     
     char hello[] = "Hello! Socket communication world!\n";
     //定义进程id
     pthread_t tid;

     int *pconnsocke = NULL;

     int ret;
     
     /*服务器端开始建立socket描述符  sockfd用于监听*/
    if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)  
    {
        fprintf(stderr,"Socket error:%s\n\a",strerror(errno));
        exit(1);
    }
    /*服务器端填充 sockaddr结构*/ 
    bzero(&server_addr,sizeof(struct sockaddr_in));
    server_addr.sin_family     =AF_INET;
    /*自动填充主机IP*/
    server_addr.sin_addr.s_addr=htonl(INADDR_ANY);//自动获取网卡地址
    server_addr.sin_port       =htons(portnumber);

    /*捆绑sockfd描述符*/ 
    if(bind(sockfd,(struct sockaddr *)(&server_addr),sizeof(struct sockaddr))==-1)
    {
        fprintf(stderr,"Bind error:%s\n\a",strerror(errno));
        exit(1);
    }
    /*监听sockfd描述符*/
    if(listen(sockfd, 10)==-1)
    {
        fprintf(stderr,"Listen error:%s\n\a",strerror(errno));
        exit(1);
    }
    //接受客户端连接，listenfd 中的半连接中有就绪的队列
    while(1)
    {
           /*服务器阻塞，直到客户程序建立连接*/
           sin_size = sizeof(struct sockaddr_in);
           //接受客户端请求产生一个新的fd
           if((new_fd = accept(sockfd,(struct sockaddr *)&client_addr,&sin_size)) == -1)
           {
                 fprintf(stderr,"Accept error:%s\n\a",strerror(errno));
                 exit(1);
           }
            printf("current fd is %d\n",new_fd);
            pconnsocke = (int *) malloc(sizeof(int));

           *pconnsocke = new_fd;  //转换成对应的参数类型  对应的fd 读写数据处理交给子线程去处理

           //子线程进程处理数据的读写请求
           ret = pthread_create(&tid,NULL,rec_func,(void *)pconnsocke);

           //判断线程是否创建成功
           if(ret == -1)
           {
                perror("pthread_create err");
           }
    }

}