#include <stdio.h>

int main()
{
     unsigned int a = 60;
     unsigned int b =13;
     //int c = a & b;
     
    // printf("Line 1 - c 的值是 %d\n", c );   //二进制的位运算 同事真则为真

     //c = a |b;
     //printf("Line 2 - c 的值是 %d\n", c );  //二进制的位运算 有一个为真则为真 二进制在转化为 十进制

     //c = a ^ b;       /* 49 = 0011 0001 */  // 二进制 异或运算
    // printf("Line 3 - c 的值是 %d\n", c );

    //c = ~a;          /*-61 = 1100 0011 */
    //printf("Line 4 - c 的值是 %d\n", c );

    //c = a << 2;     /* 240 = 1111 0000 */
   //printf("Line 5 - c 的值是 %d\n", c );

   //c = a >> 2;     /* 15 = 0000 1111 */
   //printf("Line 6 - c 的值是 %d\n", c );
   int c;
   c &=  2;
   printf("Line 9 - &= 运算符实例，c 的值 = %d\n", c );

   c ^=  2;
   printf("Line 10 - ^= 运算符实例，c 的值 = %d\n", c );
 
   c |=  2;
   printf("Line 11 - |= 运算符实例，c 的值 = %d\n", c );
}