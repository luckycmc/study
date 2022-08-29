#include <stdio.h>
//菜单 
void menu()
{
	  printf("*****************\n");
      printf("***1.sum 2.sub***\n");
      printf("***3.mul 4.div***\n");
      printf("*****0.退出*****\n");
      printf("*********\n");
} 
int add(int x, int y)
{
    return x + y;
}
int sub(int x, int y)
{
    return x - y;
}
int mul(int x, int y)
{
    return x * y;
}
int div(int x, int y)
{
    return x / y;
}

//封装一个计算的函数 只需要
void calc(int(*pf)(int,int))
{
	int x =0;
	int y =0;
	int ret = 0;
	printf("输入两个操作数");
	scanf("%d%d",&x, &y);
	ret = pf(x,y);  //函数指针根据传入的函数名调用相应的方法 
	printf("%d\n",ret); 
} 
int main()
{
	int input = 0;
	do{
		menu();
		printf("请选者:");
		scanf("%d",&input);
		//用switch 分支判断条件
		switch(input)
		{
			case 1:
				calc(add); 
				break;
			case 2:
				calc(sub);
				break;
			case 3:
				calc(mul);
				break;
			case 4:
				calc(div);
				break;
			case 0:
				printf("退出计算器！\n");
				break;
			default:
			   printf("选着错误\n");
			   break;	
		} 
	}while(input);
	return 1;
} 
