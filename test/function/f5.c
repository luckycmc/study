#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int *f(int a,int b);  //声明一个指针函数
// 主函数
int main()
{
      printf("------------------------------ Start\n");  
      
       int *p1 = NULL;
       printf("The memeory address of p1 = 0x%x \n", p1);

      p1 = f(1,3); //p1 接受 f 的返回值
      
      printf("The memeory address of p1 = 0x%x \n", p1);  
      printf("*p1 = %d \n", *p1);  

       printf("------------------------------ End\n");  

       return 0;  
}

int *f(int a,int b)
{
    int *p = (int*)malloc(sizeof(int));//申请指向空间

    printf("The memeory address of p = 0x%x \n", p);

    memset(p, 0, sizeof(int));
    *p = a+b;
    printf("*p = %d \n", *p);

    return p;
}