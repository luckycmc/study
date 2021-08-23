#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define MSG_TRY "try agin\n"

#define MSG_TIMEOUT "time out\n"
/**
  应用程序的系统调用过程
  应用程序->标库函数->系统调用->驱动->硬件
**/
int main(void)
{
    
    char buf[10];
    int fd,n,i;

    fd = open("/dev/tty",O_RDONLY|O_NONBLOCK);//打开对应的文件 非阻塞的方式打开
    if(fd < 0){
       perror("open /dev/tty");
       exit(0);
    }
    //打印输出对应的fd
    printf("open /dev/tty ok... %d\n", fd);
    for (i = 0; i < 5; i++) //数据的超时请求 超过对应的时间就自动返回退出
    {
         n = read(fd, buf, 10);
         if (n > 0) { //说明读到了东西
             break;
         }
         if (errno != EAGAIN) { //EWOULDBLOCK
            perror("read /dev/tty");
            exit(1);
         }
          else {
            write(STDOUT_FILENO, MSG_TRY, strlen(MSG_TRY));
            sleep(2);
         }
   }
   //打印输出对应的数据
    if (i == 5) {

           write(STDOUT_FILENO, MSG_TIMEOUT, strlen(MSG_TIMEOUT)); //直接打印输出
    } else {
           write(STDOUT_FILENO, buf, n);
    }

    close(fd);// 关闭fd
    return 0;
}