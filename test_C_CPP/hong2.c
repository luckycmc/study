#include <stdio.h>

int main()
{
  
    #if _WIN32
         system("color 0c");
         printf("http://c.biancheng.net\n");
    #elif __linux__
         printf("\033[22;31mhttp://c.biancheng.net\n\033[22;30m");
    #else
         printf("http://c.biancheng.net\n");          
    #endif
    
    return 0;
}
 比较两个数的大小 start*/
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