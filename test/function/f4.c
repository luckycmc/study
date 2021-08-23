
#include <stdio.h>
#include <stdlib.h>
/***
 * 第三种用结构体函数指针的方法
 * */
typedef struct gfun{
	void (*pfun)(int);	
}gfun;

void myfun(int data)
{
	printf("get data:%d\n",data);
}
int main(int argc,char *argv[])
{  
     //将函数指针指向要调用函数的地址
	/* gfun gcode={
		.pfun = myfun,  
	}; */

    gfun gcode;  //实例化结构体

    gcode.pfun = myfun;      //赋值对应的函数地址 回调函数
	gcode.pfun(100000);
	return 0;
} 
