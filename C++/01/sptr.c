#include <stdio.h>
// 函数指针也是一种指针，是指向函数的指针
 // int(*pf) (int, int) 函数指针 类型格式相同就可以 pf就是函数指针 
void test()
{
	printf("haha\a");
}

int main()
{   
    printf("%p\n",test);
	printf("%p\n",&test); 
	return 0;
}
