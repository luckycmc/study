#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int  main()
{
     pid_t pid,wpid;
     int status;

     //创建子进程
     pid = fork();
     //创建的子进程
     if (pid == 0)
     {
         printf("---child, my id= %d, going to sleep 10s\n", getpid());
         sleep(10);
         printf("-------------child die--------------\n");
         return 73;
     }else if(pid>0){  //父进程的执行代码
         
         wpid = wait(&status);
         if (wpid == -1)
         {
             perror("wait error");
             exit(1);
         }
         //进程的几种情况 
         //正常退出
         if (WIFEXITED(status))   //为真,说明子进程正常终止.
         {
             printf("child zhengchang exit with %d\n", WEXITSTATUS(status));
         }
         //说明子进程被信号终止
         if (WIFSIGNALED(status))
         {
             printf("child kill with signal %d\n", WTERMSIG(status));  //WTERMSIG(status)获取当前信号的值
         }
         
         //异常退出
         printf("------------parent wait finish: %d\n", wpid);
     }else{
         
          perror("fork");
          return -1;
     }
     return 0;
}