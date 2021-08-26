#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

void process_children();
void process_mansger();
int main()
{   
     pid_t pid;
    //创建子进程
    pid = fork();

    //子进程
    if(pid == 0){
          
          process_children();

    }else if(pid >0){  // 父进程
          process_mansger();
           
    }else if(pid < 0){ //
        printf("create process is error\n");
        exit(1);
    }
    return 1;
}
//子进程
void process_children()
{
    printf("child my parent =%d\n",getpid());
    sleep(2);
    printf("child is die!!!!!!!!!!!!!!!!\n");
}
//管理进程
void process_mansger()
{          
    pid_t pid,wpid;

    int status;
    while (1)
    {
        wpid = wait(&status); 
        if (wpid == -1)
        {
                perror("wait error");
                exit(1);
        }
        printf("parent wait finished wait children pid is %d\n",wpid);
        process_children();
    }
     
}