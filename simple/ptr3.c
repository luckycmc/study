#include <stdio.h>

const int MAX = 3;

int main()
{
	int var[] = {100,200,500};
	
	int i,*ptr[MAX]; //定义一个整型的指针数组
	
	for(i = 0;i<MAX;i++)
	{
		
		ptr[i] = &var[i]; //把元素的地址赋值给数组 
	} 
    
    for(i=0;i<MAX;i++)
	{
		printf("Value of var[%d] = %d\n", i, *ptr[i] ); //  输出元素地址指向的元素 
	}
	
	return 0;	
} 
