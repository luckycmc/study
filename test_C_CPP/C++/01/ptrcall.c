#include<stdio.h>
 
 int add(int x,int y)
 {
 	return x +y;
 } 
 
 
 int main()
 {
 	int (*p)(int,int); //定义一个函数指针  返回类型 （*函数名）（参数列表） 
	 p = add; // 把函数 add 的地址赋值给 p 赋值的时候一定要确保类型相同 
	 printf("add(10,20)=%d\n",(*p)(10,20));
    return 0; 
 }
