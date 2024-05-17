#include <stdio.h> 
#include <string.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <error.h> 
#include <errno.h> 
#include <sys/socket.h> 
#include <stdlib.h>

const char *str = "SOCKET PAIR TEST DATA";

int main()
{
     char buf[128]={0};
     int socket_pair[2];
     pid_t pid;
     int i,j;
     //创建进程管道
     if (socketpair(AF_UNIX, SOCK_STREAM,0,socket_pair) == -1)
     {
          printf("Error, socketpair create failed, errno(%d): %s\n", errno, strerror(errno));
          return EXIT_FAILURE; 
     }
     pid = fork();
     if(pid < 0) {
        printf("Error, fork failed, errno(%d): %s\n", errno, strerror(errno));
        return EXIT_FAILURE;
     } else if(pid > 0) {  //父进程空间 在管道中写数据
        
        for ( i = 0; i < 3; i++)
        {     
              //关闭另外一个套接字
              close(socket_pair[1]);
              int size = write(socket_pair[0], str, strlen(str));
              printf("Write success, pid: %d\n", getpid());
              sleep(1);
        }
        
     } else if(pid == 0) {
        
        for ( j = 0; j < 3; i++)
        {  
            //关闭另外一个套接字
            close(socket_pair[0]);
            read(socket_pair[1], buf, sizeof(buf));  
            printf("Read result: %s, pid: %d\n",buf, getpid()); 
        }     
        
     }


     for(;;) {
        sleep(1);
    }

     return 1;
    
}