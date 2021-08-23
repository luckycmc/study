#include <stdio.h>

/**
  指针函数传参数 
*/
void test(int *p);

int main()
{
	int num = 90;
	int *p;
	
	p = &num;
	test(&num); //传递其变量的地址 
	
	test(p);
	
	printf("num is:%d\n",num);
}

void test(int *p)
{
	*p+=1;
}
