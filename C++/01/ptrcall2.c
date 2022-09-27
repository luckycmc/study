#include<stdio.h>

typedef struct TFT_DISPLAY
{
	int x;
	int y;
	void (*p)(void); //打印函数sss 
	int (*m)(int,int);
}tft_display;

//打印函数

void display(void)
{
	printf("display function\n");
} 

// 加法函数

int sum(int x,int y){
	return x+y;
} 

tft_display stu = { //结构体赋值操作 
	.x = 1,
	.y = 2,
	.p = display,  //指针函数赋值 只要参数类型 返回值相同 就可以进行指向 
	.m = sum,
};
//数据初始化 
int main()
{
	stu.p();
	printf("sub is %d\n",stu.m(stu.x,stu.y));
	return 1;
}
