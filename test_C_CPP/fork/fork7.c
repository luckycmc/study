#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

//错误打印函数
void sys_err(const char *str)
{
    perror(str);
    exit(1);
}

int main()
{
    int ret;
    int fd[2];
    pid_t pid;

    char *str ="hello pipe\n";
    char buf[1024];

    //创建管道
    ret = pipe(fd);
    
    if (ret == -1) //管道创建失败
    {
         sys_err("pipe create error\n");   
    }
    
    //创建子进程
    pid = fork();
    //父进程执行的代码段
    if(pid > 0){
        //父进程关闭读段
        close(fd[0]);
        sleep(3);
        
        write(fd[1],str,strlen(str));
        //关闭写段
        close(fd[1]);

    }else if(pid == 0){  //子进程执行的代码段
      
        //子进程关闭写段
        close(fd[1]);
        ret = read(fd[0],buf,sizeof(buf));
        printf("children read: ret = %d\n",ret);
        //打印到客户端
        write(STDOUT_FILENO,buf,ret);
        close(fd[0]); //关闭写段
       
    }
    return 0;
}