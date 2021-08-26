#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define PROCESS_NUM  3

/****
 * 创建多进程管理
 * */
pid_t worker_pid[3];

static pid_t process_children();
void process_mansger();
//主函数
int main()
{  
    pid_t pid,new_pid,manager_tid;
    pid = fork();
    int i;
    switch (pid)
    {
          case 0: //子进程空间
            for (i = 0; i <PROCESS_NUM;i++)
            {
                new_pid = process_children();
                if (new_pid < 0)
                {
                     return -1;
                }else{
                    worker_pid[i] = new_pid;
                }
                
            }
            process_mansger();
            break;
          default:
             manager_tid = pid;
             break;

          case -1:
            exit(1);
            break;
    }
    return 1;
}

//子进程
static pid_t process_children()
{         

    pid_t pid = fork();
    if (pid < 0)
    {
        return -1;
    }
    else if (pid > 0)
    {   
        return pid;

    }else if(pid == 0)
    {        
        printf("child pid is =%d\n",getpid());
        sleep(2);
        printf("child is die!!!!!!!!!!!!!!!!\n");
        exit(1);
    }
}
//管理进程
void process_mansger()
{   
    printf("parent pid is =%d\n",getpid());
    pid_t wpid;
    
    int status,i;
    while (1)
    {
        wpid = wait(&status); 
        if (wpid == -1)
        {
               continue; //没有回收的继续循环
        }else{
              //打印回收后的数据
              printf("parent wait finished wait children pid is %d\n",wpid);
        }  
       
    }
     
}