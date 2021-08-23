#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
/**
 * @1.子进程先结束
 * @2.父进程在进程回收
 * */
int main()
{
    pid_t pid,wpid;
    int status;
    //创建进程
    pid = fork();
    if(pid ==0){  //子进程的执行空间
       
        printf("child  ,mys parent = %d. going to sleep for 5s seconds\n",getpid());
        sleep(5);
        printf("child die!!!!!!!\n");

    }else if(pid > 0){  //父进程执行的空间
            
             //父进程回收子进程
             wpid = wait(&status);
             /*************等待进程 退出 start*****************/
             if(wpid == -1){
                 perror("wait error");
                 exit(1);
             }
             /*************等待进程 退出 end*****************/
             printf("parent wait finish\n");
    }else{
        perror("fork error\n");
        return -1;
    }

    return 0;
}
