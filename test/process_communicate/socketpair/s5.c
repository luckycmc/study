#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
/** 
 * pipie 也可以用于线程间通讯
 * 
 * */
// 创建管道结构体
struct pipe_rw
{
   int fd_r;  //读管道
   int fd_w;  //写管道
};
//线程函数
void *thread_handle(void *arg)
{
    //实例化指针结构体
    struct pipe_rw *pPipeRw = (struct pipe_rw *)arg;
    char buf[512];
    int len;
    //循环读写数据
    while(1)
    {
          //线程从管道中读取数据
          len = read(pPipeRw->fd_r,buf,512);
          buf[len] = '\0';
          printf("%s\n",buf);
          //格式化数据处理
          len = sprintf(buf,"hello my father");
          buf[len] = '\0';
          //写入写管道
          write(pPipeRw->fd_w,buf,len);
          sleep(3);
    }
}
//主函数
int main()
{
    int fd[2];   //fd[0]是读，fd[1]是写
    int fd2[2];
    int res = pipe(fd);
    if (res) {
       printf("create pipe error\n");
       return -1;
    }
    res = pipe(fd2);
    if (res) {
       printf("create pipe2 error\n");
       return -1;
    }
    pthread_t thread; //线程id 变量

    struct pipe_rw pipe_arg;  //创建结构体

    pipe_arg.fd_r = fd[0];   //读管道
    pipe_arg.fd_w = fd2[1];  //写管道
    //创建对应的线程
    pthread_create(&thread, NULL, thread_handle, &pipe_arg);
    char buf[512];
    int len;
    while (1) {
        len = sprintf(buf, "hello my child");
        buf[len] = '\0';
        //写数据
        write(fd[1], buf, len);
        //读数据
        len = read(fd2[0], buf, 512);
        buf[len] = '\0';
        printf("%s\n", buf);
        sleep(3);
    }
    return 0;
}
// 参考 https://www.cnblogs.com/zhu-g5may/p/10555238.html