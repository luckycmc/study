#include <stdio.h>
#include <stdlib.h>
// 也可以用typedef来定义一个指针函数这样使在大型代码中更加简洁
typedef void (*pfun)(int data);
/*typedef的功能是定义新的类型。第一句就是定义了一种pfun的类型，并定义这种类型为指向某种函数的指针，
这种函数以一个int为参数并返回void类型。*/
void myfun(int data)
{
	printf("get data:%d\n",data);
}
/** 
 * 这里面的pfun代表的是函数的类型，通过pfun来代表void (*)(int)函数类型即pfun是指针函数的别名，pfun p相当于定义了一个
void (*p)(int)函数指针。p = myfun可以理解为将函数指针p指向myfun函数的地址，p(100);相当于执行myfun(100);
 * */
int main(int argc,char *argv[])
{
	pfun p= myfun;      //函数指针指向执行函数的地址
	p(1000);
     printf("method adressing %p\n",p);
	return 0;
}
