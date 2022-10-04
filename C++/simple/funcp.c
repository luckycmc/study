#include <stdio.h>

int max(int a,int b)
{
	return a > b ? a:b;
}

int main()
{
	//p 是指针函数
	int (*p)(int,int) = &max;//&可以省略
	int a,b,c,d;
	
	printf("请输入三个数字:");
	scanf("%d %d %d",&a,&b,&c);
	
	d = p(p(a,b),c);
	
	printf("最大的数字是:%d\n",d);
	return 0; 
}
