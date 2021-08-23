#include <stdio.h>
#include <time.h>
#include <stdlib.h>

/**
  返回指针函数 
*/

int *getRandom()
{
	static int r[10];
	
	int i;
	/*设置种子*/
	srand((unsigned)time(NULL)); 
	
	for(i=0;i<10;i++)
	{
	    r[i] = rand();
		printf("%d\n",r[i]);	
	}
	return r; //数组元素的首地址 
} 

//定义一个主函数

int  main()
{
	//定义一个指针函数
	int *p;
	int i;
	p = getRandom();  //返回的是数组的首地址  一次变量  整型指针变量p是可以接受的  
	for(i=0;i<10;i++)
	{   
	    printf("****%p\n",(p + i)); //输出对应的地址 
		printf("*(p + [%d]) : %d\n", i, *(p + i) );
	}
	return 0; 
} 
