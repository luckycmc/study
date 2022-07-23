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
int main()
{
	Printss();
    return 0;
}
