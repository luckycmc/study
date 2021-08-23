#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*C语言中数据的内存分配*/
int a = 0; 
char *p1; 

int main()
{
   int b;                  /* b在栈 */
    char s[] = "abc";           /* s在栈， "abc"在常量区 */
    char *p2;                   /* p2在栈 */
    char *p3 = "123456";        /*"123456"在常量区，p3在栈*/
    static int c =0;            /*可读可写数据段*/
    p1 = (char *)malloc(10);    /*分配得来的10个字节的区域在堆区*/
    p2 = (char *)malloc(20);    /*分配得来的20个字节的区域在堆区*/
    /* 从常量区的“Hello World”字符串复制到刚分配到的堆区 */
    strcpy(p1, "Hello World\n");
    printf("%s",p1);
    return 0;
}