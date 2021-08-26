#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

void process_children();
void process_mansger();
int main()
{   
        pid_t pid,wpid;
         
        int i;
          
        for ( i = 0; i < 3; i++)
        {    
            pid = fork();
            printf("------------------pid is %d\n", pid);
              // 这是异常情况
            if (pid==-1)
            {
                perror("fork失败!");
                exit(1);
            }
            //循环中，fork函数调用五次，子进程返回0,父进程返回子进程的pid，
            //为了避免子进程也fork，需要判断并break
            if (pid == 0)
            {
                   break;
            }
             
        }
        // 父进程
        if (pid > 0)
        {
             //回收子进程
             process_mansger();
        }
        else if (pid == 0)
        {
            //创建子进程
            process_children(); 
        }
        
        return 1;
}
//子进程
void process_children()
{   

          printf("child pid is =%d\n",getpid());
          sleep(10);
          printf("child is die!!!!!!!!!!!!!!!!\n");
}
//管理进程
void process_mansger()
{   
    printf("parent pid is =%d\n",getpid());
    pid_t pid,wpid;
    
    int status;
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