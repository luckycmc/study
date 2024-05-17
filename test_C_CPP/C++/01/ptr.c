#include <stdio.h>
#include <stdlib.h> 
void Printss()
{
    //栈空间是 高地址向低地址增长 
    int  a, b, dA, dB;
    printf("a 的地址：%p \n", &a);
    printf("b 的地址：%p \n", &b);
    printf("dA的地址：%p \n", &dA);
    printf("dB的地址：%p \n\n\n", &dB);
    printf("ssss");
    
}
struct A {
	int id;
};
int main()
{
	//Printss();
	struct A *a,*b;
	a = (struct A *)malloc(sizeof(struct A));
	a->id =100;
	
	b = (struct A *)malloc(sizeof(struct A));
	b->id =101;
	 printf("a 的地址：%p \n", &a);
    printf("b 的地址：%p \n", &b);
    return 0;
}
