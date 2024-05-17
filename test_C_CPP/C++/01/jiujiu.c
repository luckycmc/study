#include<stdio.h>
/**
  实现 9*9 乘法表 
**/
int main()
{
	//首先需要程序数据 和结果
	int i,j,result;
	printf("\n"); //换行输出
	//第一轮输出 9个数据 
	for(i=1;i<10;i++)
	{
		for(j=1;j<=i;j++) //例如第一行 1*1 1*2 *2.... 
		{
			 //计算对应的结果
			 result = i * j;
			 printf("%d*%d=%-3d",j,i,result); /*-3d表示左对齐，占3位*/ 
		} 
		//每一行换一个行
		printf("\n"); 
	} 
	return 0;
} 
