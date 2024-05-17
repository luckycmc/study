#include <stdio.h>


//元素进栈
int push(int *a, int top,int elem)
{
     a[++top] = elem;
      printf("入栈栈元素---%d\n",a[top]);
     return top; //返回入栈的下标
}
//元素出栈
int pop(int *a, int top)
{
    //判断是否是空栈
    if (top == -1)
    {
        printf("空栈\n");
        return -1;
    }
    
    printf("弹栈元素---%d\n",a[top]);
    top--;
    return top;
}
//主函数执行
int main()
{
    int a[100];
    int top=-1;
    top=push(a, top, 1);
    top=push(a, top, 2);
    top=push(a, top, 3);
    top=push(a, top, 4);
    /*********出栈*****************/
    top=pop(a, top);
    top=pop(a, top);
    top=pop(a, top);
    top=pop(a, top);
    top=pop(a, top);
    return 0;
}
// /configure --with-php-config=/usr/local/php/bin/php-config