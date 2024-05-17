#include <unistd.h>
#include <stdio.h>

/**
  而管道一般是半全工通信，要双全工就得创建2个管道
  pipe()函数创建管道 用于进程通讯
  道是一种把两个进程之间的标准输入和标准输出连接起来的机制，从而提供一种让多个进程间通信的方法，当进程创建管道时，每次

都需要提供两个文件描述符来操作管道。其中一个对管道进行写操作，另一个对管道进行读操作。对管道的读写与一般的IO系统函数一

致，使用write()函数写入数据，使用read()读出数据。 
 * @return int 
 */
int main()
{
      int fd[2];
      int fd1[2];

      //创建管道
      int res = pipe(fd);
      if (res == -1)
      {
           printf("create pipe error\n");
           return -1;
      }
      res = pipe(fd1); //创建管道2
      if (res == -1) {
            printf("create pipe2 error\n");
            return -1;
     }
     //创建进程
     pid_t pid = fork();
     //父进程
     if (pid >0)
     {  
         close(fd[0]);
         close(fd1[1]);
         char buf[512];
         int len;
        while (1) {
            len = sprintf(buf, "hello my child!");
            buf[len] = '\0';
            write(fd[1], buf, len);    
            len = read(fd1[0], buf, 512);
            buf[len] = '\0';
            printf("%s\n", buf);
            sleep(3);
        }         
     }else if (pid ==0){ // 子进程
         close(fd[1]);
         close(fd1[0]);   
       char buf[512];
       int len;
       while (1) {
           len = read(fd[0], buf, 512);    //读取数据
           buf[len] = '\0';
           printf("%s\n", buf);
           len = sprintf(buf, "hello my father!");
           buf[len] = '\0';
           write(fd1[1], buf, len);   //写入数据
           sleep(3);
       }
     }else if (pid <0){  //进程创建失败

         printf("fork error\n");
         return -1;
     }
     
    return 1;
}