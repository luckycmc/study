#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>

int main()
{
     int i;
     pid_t pid,wpid;
     
     for ( i = 0; i < 5; i++)
     {
         pid = fork();
         //子进程 不参与创建进程 不然会创建2的n次方-1 个进程
         if (pid == 0)
         {
            break;
         }
         
     }
     // 进程创建完毕 回收子进程
     if (i == 5)
     {
          
            while ((wpid = waitpid(-1, NULL, WNOHANG)) != -1){  //使用非阻塞的方式回收子进程
                   
                    if (wpid > 0) {  // wpid 返回的当前回收进程的pid

                       printf("wait child %d \n", wpid);

                   } else if (wpid == 0) {
 
                      continue;
                    }
            }
     }else{ //打印创建的子进程
         
          sleep(i);
          printf("I'm %dth child, pid= %d\n", i+1, getpid());
     }
     return 1;
}
     
     