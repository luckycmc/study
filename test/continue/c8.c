#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ucontext.h>

ucontext_t ctx_main, ctx_f1, ctx_f2; //创建两个结构体变量

void fun1( void ) {
	printf("fun1() start\n");
	sleep(1);
    //切换执行顺序
	swapcontext(&ctx_f1, &ctx_f2);//切换至f2上下文
	printf("fun1() end\n");
}

void fun2( void ) {
	printf("fun2() start\n");
    //切换执行顺序
	swapcontext(&ctx_f2, &ctx_f1);//切换回f1上下文
	printf("fun2() end\n");
}

int main( void ) {
	char stack1[1024*8];
	char stack2[1024*8];
	
	getcontext(&ctx_f1);
	getcontext(&ctx_f2);
   /*******************修改对应的上下文信息 start********************/
	ctx_f1.uc_stack.ss_sp    = stack1;
	ctx_f1.uc_stack.ss_size  = 1024*8;
	ctx_f1.uc_stack.ss_flags = 0;
	ctx_f1.uc_link = &ctx_f2;//f1设置后继上下文为f2
	makecontext(&ctx_f1, fun1, 0);//设置入口函数

	ctx_f2.uc_stack.ss_sp    = stack2;
	ctx_f2.uc_stack.ss_size  = 1024*8;
	ctx_f2.uc_stack.ss_flags = 0;
	ctx_f2.uc_link = &ctx_main;//f2后继上下文为主流程
	makecontext(&ctx_f2, fun2, 0);//设置入口函数
    /*******************修改对应的上下文信息 start********************/

	swapcontext(&ctx_main, &ctx_f1);//保存ctx_main,从主流程上下文切换至ctx_f1上下文

	printf("main exit\n");
}
