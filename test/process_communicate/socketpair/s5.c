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
}
//线程函数
void *thread_handle(void *)
{

}
//主函数
int main()
{

}
// 参考 https://www.cnblogs.com/zhu-g5may/p/10555238.html