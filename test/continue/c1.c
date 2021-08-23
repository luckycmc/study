#include<stdio.h>
#include<ucontext.h>
#include<unistd.h>
/*
  ucontext来实现简单的协程库。
  利用ucontext提供的四个函数getcontext(),setcontext(),
  makecontext(),swapcontext()可以在一个进程中实现用户级的线程切换。
*/
/**
  1.结构体
  typedef struct ucontext {
               struct ucontext *uc_link;
               sigset_t         uc_sigmask;
               stack_t          uc_stack;
               mcontext_t       uc_mcontext;
               ...
           } ucontext_t;

           当当前上下文(如使用makecontext创建的上下文）运行终止时系统会恢复uc_link指向的上下文；
           uc_sigmask为该上下文中的阻塞信号集合；uc_stack为该上下文中使用的栈；
           uc_mcontext保存的上下文的特定机器表示，包括调用线程的特定寄存器等。
    2.对应的操作函数
      int getcontext(ucontext_t *ucp);
      初始化ucp结构体，将当前的上下文保存到ucp中

     int setcontext(const ucontext_t *ucp);
     设置当前的上下文为ucp，setcontext的上下文ucp应该通过getcontext或者makecontext取得，
     如果调用成功则不返回。如果上下文是通过调用getcontext()取得,程序会继续执行这个调用。
     如果上下文是通过调用makecontext取得,程序会调用makecontext函数的第二个参数指向的函数，
     如果func函数返回,则恢复makecontext第一个参数指向的上下文第一个参数指向的上下文
     context_t中指向的uc_link.如果uc_link为NULL, 则线程退出。
   
    简单说来，  getcontext获取当前上下文，setcontext设置当前上下文，
     swapcontext切换上下文，makecontext创建一个新的上下文。
   
*/
/**
   协成的实现原理
      进程，线程:被动调度，上下文切换，栈都是系统维护
      协成：主动让出控制权，上下文切换，栈维护要自己搞

    因此协成实现的关键
       上下文的维护
       栈的维护  栈是C语言运行的基础
       栈 ：函数内部的临时变量，参数，返回值
*/
int  main()
{
     ucontext_t context; //创建协成变量

     //保存当前协成的上下文
     getcontext(&context);
     puts("Hello world");
     printf("协成切换\n");
     //模拟切换
     sleep(1);
      printf("协成恢复\n");
     //恢复当前协成的上下文
     /**
      。其实是程序通过getcontext先保存了一个上下文,然后输出"Hello world",
      在通过setcontext恢复到getcontext的地方，重新执行代码，
      所以导致程序不断的输出”Hello world“
     */
     setcontext(&context);
     return 0;

}