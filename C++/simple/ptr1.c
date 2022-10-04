#include <stdio.h>

int main()
{
	int var_runoob = 10;
	
	int *p; //定义一个整型类型的指针变量 
	
	p = &var_runoob; // 指针变量p  获取  var_runoob 的地址
	
    printf("var_runoob 变量的地址： %p\n", p);
    
    //访问指针变量的值
	printf("*p 的值是 %d\n",*p); 
	
	/***************指针默认为NULL********************/
	int *p3 = NULL;
	
	printf("p3 的地址是%p\n",p3);
	return 0; 
	
}
