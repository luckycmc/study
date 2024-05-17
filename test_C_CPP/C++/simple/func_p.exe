#include <stdio.h>

int Max();  //函数声明 

//  int(*p)(int, int);  括号里面的首先是个指针 指向的为函数 返回的类型为整型 
int main()
{   
     
    int (*p)(int,int);  //定义一个函数指针 
	
	int a,b,c;
	
	p = Max;  //指针p 指向函数的首地址 
	
	printf("please enter a and b:");
	
    scanf("%d%d", &a, &b);
	
	c = (*p)(a, b);  //通过函数指针调用Max函数
	
    printf("a = %d\nb = %d\nmax = %d\n", a, b, c);
    
	return 0;
}

//返回函数的最大值 
int Max(int x,int y)
{
	return x > y ? x : y; 
} 
