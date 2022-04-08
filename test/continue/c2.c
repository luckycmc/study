#include <ucontext.h>
#include <stdio.h>

//定义一个函数
void func1(void *arg)
{
   
    puts("1");
    puts("11");
    puts("111");
    sleep(30);//模拟IO等待
    puts("IO 等待 两秒");
    puts("1111");
}
//测试上下文 其实就是从一个线程的 执行流切换到另一个线程的执行流
// 保存当前的协程的用户信息 用于恢复
void  context_test()
{
     char stack[1024*128];
     ucontext_t child,main;  //创建上下文结构体

     //获取当前下上文
     getcontext(&child);

     child.uc_stack.ss_sp = stack; //指定栈的空间 的开始位置 也就是sp指针
     printf("start ip指针 ---%p\n",stack);

     child.uc_stack.ss_size = sizeof(stack);//指定栈空间的大小
     //栈的结束位置
     printf("end ip指针 ---%p\n",&child.uc_stack.ss_size);
     child.uc_stack.ss_flags = 0;//指定空间的位置
     // 如果要实现主函数到线程的切换并返回，指定后继上下文是非常重要的。
     child.uc_link = &main; //设置后继上下文  去掉了后继上下文 就不在执行

    makecontext(&child,(void (*)(void))func1,0); //修改上下文指向func1函数 也就是产生一个协成
 
    swapcontext(&main,&child);//切换到child上下文，保存当前上下文到main
    puts("main");//如果设置了后继上下文，func1函数指向完后会返回此处

}
//主函数
int main()
{  
    printf("主线程执行\n");
    context_test();//协成上下文测试
    return 1;
}