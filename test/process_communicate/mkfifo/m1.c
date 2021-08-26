#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
//fifo 可以用于无血缘关系的进程通讯
/*命名管道： mkfifo 
无血缘关系进程间通信：
读端，open fifo O_RDONLY
写端，open fifo O_WRONLY
*/
void sys_err(const char *str)
{
    perror("sys_err");
    exit(1);
}

int main()
{
    int ret = mkfifo("testinfo",0664);
    if (ret == -1)
    {
        sys_err("mkfifo error");
    }

    return 0;
}