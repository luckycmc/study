#include <stdio.h>
#include <stdlib.h>
//定义一个函数指针
void (*pfun)(int data);  // 返回值为空类型  指向对应的函数 的话 返回值 和 参数必须一致
//定义对应的函数
void myfun(int data)
{
	printf("get data:%d\n",data);
}
/**
 * @brief 
 * 从这个例子可以看到，我们首先定义了一个函数指针pfun ,这个函数指针的返回值为void型,然后我们给函数指针赋值，
 * 赋值为myfun,也就是myfun函数的首地址，在C99中myfun函数名就是myfun函数的首地址，此时pfun获得了myfun的地址，
 * pfun的地址等于myfun的地址，所以最终调用pfun();也就相当于调用了myfun();
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc,char *argv[])
{
	pfun = myfun;  // 获取 myfun的函数地址
	(*pfun)(1000);
    printf("method adressing %p\n",myfun);
	return 0;
}
