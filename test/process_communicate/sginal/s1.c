#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <signal.h>
/**
 * @brief 
 *  kill 函数和kill命令
 *  这里子进程不发送 kill 信号，发其他信号也行，比如段错误什么的
 */
int main()
{
    pid_t pid = fork();

    if (pid > 0)//父进程空间
    {
        printf("parent ,pid = %d\n", getpid());
        while(1);

    }else if (pid == 0){ //子进程空间
        
         printf("child pid = %d,ppid = %d\n", getpid(),getppid()); 
         sleep(2);
         //给父进程发信号
         kill(getppid(), SIGKILL);
    }
    return 0;
}