#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h> //exit();
/**
 * 1 创建进程，资源代码都复制一份
 * 2.所以 i=100 会打印两次一次是 父进程的一次是子进程的
 * 3.子进程和父进程的打印顺序可能不太一样
 * */
int main()
{
    int i = 100;
    printf("start main\n");
    pid_t pid = fork();
    //printf("i=%d\n", i);
    /*主父进程的返回pid可能不同  
      1.父进程的返回值是子进程的pid
      2.子进程的返回值是0
    */
    //printf("pid=%d\n", pid);
    /*************通过fork 的值来区分不通的进程执行的方向**********/
    if (pid==0)
    {
         printf("This is child process!\n");
         //子进程退出
         exit(0);
    }
    int status;
    waitpid(&status);
    printf("status=%d\n", status);
    printf("This is parent process!\n");
    printf("just a test\n");
    return 0;
}