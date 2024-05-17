#include "multi_thread.h"

/**
  伪代码如下
   thread_func()
   {
        while(1) {
            recv(...);
            process(...);
            send(...);
        }
        close(...);
 }
main(
    socket(...); 
    bind(...);
    listen(...);
    while(1) { 
    accept(...);
    pthread_create();
    }
}
由上可以看出，服务器分为两部分：主线程、子线程
    main函数即主线程，它的主要任务如下：

    socket()创建监听套字；
    bind（）绑定端口号和地址；
    listen（）开启监听；
    accept（）等待客户端的连接，
    当有客户端连接时，accept（）会创建一个新的套接字new_fd；
    主线程会创建子线程，并将new_fd传递给子线程。
    子线程
    子线程函数为thread_func（），他通过new_fd处理和客户端所有的通信任务。
    当前服务器属于一请求 一线程服务器
 

 /**************理论基础 start*****************/
 /**
 2.使用线程模型开发服务器时需考虑以下问题：

    调整进程内最大文件描述符上限
    线程如有共享数据，考虑线程同步
    服务于客户端线程退出时，退出处理。（退出值，分离态）
    系统负载，随着链接客户端增加，导致其它线程不能及时得到CPU

3.线程共享:
      全局数据区
      堆区
      一块有效内存的地址
**/

int main()
{   
    //测试函数
    master_thread();

    return 0;
}