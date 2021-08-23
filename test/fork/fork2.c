#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    int i;
    pid_t pid;  //进程指点的pid
    printf("xxxxxxxxxxxxxx\n");
    //创建5个子进程
    for(i=0;i<5;i++)
    {

           pid = fork();//创建一个进程
           if (pid <0)
           {
               perror("fork error:\n");
               exit(1);
           }
           else if (pid == 0) // 子进程   子进程会继续创建进程  因此为2的5次方 -1 个进程
           {
               printf("I'm child,pid = %u,ppid = %u\n",getpid(),getppid());
           }
           else{
               printf("I'm parent,pid = %u, ppid = %u\n",getpid(),getppid());
               sleep(1);
           }
                  
    }

    printf("YYYYYYYYYYY\n");
    return 0;
}