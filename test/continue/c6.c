#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ucontext.h>


int main()
{ 
    
     int i = 0;
     int b = 9;
     ucontext_t ctx;

     //获取当前的执行的上下文 并保存起来

     getcontext(&ctx);

     printf("i = %d\n",i++);
     printf("b = %d\n",b++);
     sleep(1);  //延时两秒执行

     setcontext(&ctx); //将上下文恢复到设置时的状态,完成死循环
    printf("main end \n");
     return 1;
}