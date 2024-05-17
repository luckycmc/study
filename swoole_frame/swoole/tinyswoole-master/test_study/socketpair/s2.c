#include <stdio.h> 
#include <string.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <error.h> 
#include <errno.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
  
const char* str = "SOCKET PAIR COMMUNICATE";

int main()
{
    char buf[128] = {0};
    int sfd[2]; //数组
    int ret; pid_t pid;
    //创建管道 
    ret = socketpair(AF_UNIX,SOCK_STREAM,0,sfd);
    if(ret == -1){
         
          printf("create socketpair is error\n");
          return -1;
    }
    //不用的文件描述符一定要释放掉 不然 会存在当前进程中
    //创建进程
    pid = fork();
    //进程创建错误
    if(pid == -1){
        printf("create process is error\n");
        return -1;
    }else if(pid == 0){  //子进程空间
       //关闭 另一端的套接字
       close(sfd[0]);
       int size = write(sfd[1],str,strlen(str));
        printf("Write success, pid: %d\n", getpid());
    }else if(pid > 0){ //   
         close(sfd[1]);
         int size = read(sfd[0],buf,strlen(str));
         printf("Read result: %s, pid: %d\n",buf, getpid());
    }
     for(;;) {
        sleep(1);
    }
  
    return 1;
}
