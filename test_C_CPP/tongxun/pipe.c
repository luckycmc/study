/**
  1.pipe创建的管道 读完了会删除;队列
  2.如果管道中没有数据则会阻塞
  3.无名管道只能用于 父子间的进程通讯

  我们的内核利用文件描述符来访问文件，每个文件描述符都是非负整数，
  打开现存的文件或者是新建文件时，内核会返回一个文件描述符，
  读写文件也需要使用文件描述符来指定待读写的文件

  PCB中的文件描述符
  文件描述符：在linux系统中打开文件就会获得文件描述符，它是个很小的非负整数。
  每个进程在PCB（Process Control Block）中保存着一份文件描述符表，
  文件描述符就是这个表的索引，每个表项都有一个指向已打开文件的指针。
  https://www.jb51.net/article/221035.htm
  https://segmentfault.com/a/1190000009724931
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int main()
{  
   
    int fd = open("./myfile.txt",O_RDONLY);
    if(fd < 0){
        perror("open");
        return 1;
    }
    printf("fd:%d\n",fd);
    //关闭文件描述符
    close(fd);
    write(1,"hello\n",6);
    return 0;
}