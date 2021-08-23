#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ucontext.h>

//测试函数

void fun(){
 
    printf("fun()\n");
    //printf("params is %d\n",(int *)a);
}

//主函数
int main()
{
    int i = 1;
    //申请栈空间
    char  *stack = (char * )malloc(sizeof(char) * 8192);

    //设置另个环境变量
    ucontext_t ctx_main,ctx_fun;

    //获取两者的上下文
    getcontext(&ctx_main);
    getcontext(&ctx_fun);

    printf("i=%d\n", i++);
	sleep(1);

    //修改 ctx_fun 的上下文
    //设置上下文的栈信息
	ctx_fun.uc_stack.ss_sp    = stack;
	ctx_fun.uc_stack.ss_size  = 8192;
	ctx_fun.uc_stack.ss_flags = 0;
	ctx_fun.uc_link = &ctx_main;//设置ctx_main为ctx_fun的后继上下文
    
    //修改上下文信息
    makecontext(&ctx_fun,fun,0); //修改上下文信息，设置入口函数与参数
    //恢复上下文
    setcontext(&ctx_fun);//恢复ctx_fun上下文
	
	printf("main exit\n");
    return 0;
}