#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>         
#include <sys/socket.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

int main()
{
    int fd[2];
    int ret;
    ret = socketpair(AF_UNIX, SOCK_SEQPACKET, 0, fd);
    
    if(ret < 0){
        perror("socketpair");
        return 1;
    }
    pid_t pid = fork();
    if(pid < 0){

         perror("fork");
         return 2;

    }else if(pid == 0){  //子进程空间
          
          close(fd[0]);
          char buf[1024];
          while(1){
              
               ssize_t _s;
            strcpy(buf,"hello bit");
            write(fd[1],buf,strlen(buf));
            _s=read(fd[1],buf,sizeof(buf)-1);
            buf[_s]='\0';
            printf("father->child%s\n",buf);
          }
          //使用玩关闭
          close(fd[1]);

    }else{ //父进程空间
        close(fd[1]);
        char buf[1024];
        while(1)
        {
            ssize_t _s = read(fd[0],buf,sizeof(buf)-1);

            if(_s>0){
                buf[_s]='\0';
                printf("child -> father %s\n",buf);
            }

            strcpy(buf,"hello world");
            write(fd[0],buf,strlen(buf));
        }              
        close(fd[0]);
        //wait child
    }
}