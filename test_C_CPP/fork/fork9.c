#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
     int fd[2];
     //创建 socketpair 双向管道  socketpair产生的套接字对实现全双工通信
     int r = socketpair(AF_UNIX, SOCK_STREAM, 0, fd);
     if (r <0)
     {
         perror("socketpair()");
         exit(1);
     }
     pid_t pid;

     pid = fork();


     if (pid > 0)  //主进程
     {
          int val =  0;
          //关闭写通道
          close(fd[1]);
          while(1){
               sleep(1);
               ++val;
               printf("%d child->发送数据: %d\n", getpid(), val);
               write(fd[0],&val,sizeof(val));
               read(fd[0],&val,sizeof(val));
               printf("%d child->接收数据: %d\n", getpid(), val);
          }
     }else if (pid == 0) //子进程
     {
            int val;
            close(fd[1]);
            while(1){

                read(fd[1], &val, sizeof(val));
                printf("%d father->接收数据: %d\n", getpid(), val);
                ++val;
                printf("%d father->发送数据: %d\n", getpid(), val);
                write(fd[1], &val, sizeof(val));

            }
     }else{
          
            perror("create process is error\n");
            exit(1);
     }
     
     
}