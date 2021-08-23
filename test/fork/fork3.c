#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main()
{
    pid_t pid;
    int i;
    printf("##############\n");
    for ( i = 0; i < 5; i++)
    {
        pid = fork();
        if (pid == 0) //子进程要需要退出
        {
            break;
        }
        
    }
    
    if(i < 5)  //子进程执行空间
    {
        sleep(i);
        printf("I'm %d child,pid = %u\n",i+1,getpid());

    }
    else   //父进程的空间
    {
        sleep(i);
        printf("I'm parent\n");

    }
    return 0;
}