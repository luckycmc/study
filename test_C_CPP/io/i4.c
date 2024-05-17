#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

int main()
{
    int fd,n;
    char buf[] = "It's a test for lseek\n";
    char ch;
    fd = open("./lseek.txt",O_RDWR|O_CREAT, 0644);
    if(fd < 0){
        
           perror("open lseek.txt error");
           exit(1);
    }

    write(fd,buf,sizeof(buf));
    lseek(fd, 0,SEEK_SET);    //修改文件读写指针的位置 lseek 移动指针的位置 表示 当前的fd从那个位置开始读的

    while(n =read(fd,&ch,1)){
           
           if(n < 0){
                perror("read error");
                exit(1);
           }
           write(STDOUT_FILENO, &ch, n); //将文件内容按字节读出，写出到屏幕
    }
    close(fd);
    return 0;
}