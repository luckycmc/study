#include <stdio.h>
#include <stdlib.h> 
void Printss()
{
    //ջ�ռ��� �ߵ�ַ��͵�ַ���� 
    int  a, b, dA, dB;
    printf("a �ĵ�ַ��%p \n", &a);
    printf("b �ĵ�ַ��%p \n", &b);
    printf("dA�ĵ�ַ��%p \n", &dA);
    printf("dB�ĵ�ַ��%p \n\n\n", &dB);
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
	 printf("a �ĵ�ַ��%p \n", &a);
    printf("b �ĵ�ַ��%p \n", &b);
    return 0;
}
