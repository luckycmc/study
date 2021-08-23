#include <stdio.h>


#define N 100   //宏定义N 为10

#define M(y) y*y+3*y //宏定义

#define MAX(a,b) (a>b) ? a :b


int SQ(int y){
  return ((y)*(y));
}

int main()
{
     int sum = 20 + N;
     printf("%d\n",sum);

     int k = M(5);
     printf("带参数的宏定义%d\n",k);

     /*宏定义 比较两个数的大小 start*/
     int x,y,max;
     printf("input two numbers:");
     scanf("%d %d",&x,&y);
     max = MAX(x, y);
     printf("max=%d\n", max);
     /*宏定义 比较两个数的大小 end*/

     /** 
       带参数的宏和函数很相似，但有本质上的区别：
       宏展开仅仅是字符串的替换，不会对表达式进行计算；
       宏在编译之前就被处理掉了，它没有机会参与编译，也不会占用内存。
       而函数是一段可以重复使用的代码，会被编译，会给它分配内存，每次调用函数，就是执行这块内存中的代码。
      * */
    

     return 0;
}