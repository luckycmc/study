#include <stdio.h>

int max(int a, int b) {  
    return a > b ? a : b;  
}  

int min(int a, int b) {  
    return a < b ? a : b;  
}

// 声明函数指针  指向的时候要根据对应的类型对应上
int (*f)(int, int); // 声明函数指针，该指针指向返回值类型为int，有两个参数类型都是int的函数  
//主函数
int main()  
{  
    printf("------------------------------ Start\n");  

    f = max;    // 函数指针f指向求较大值的函数max   对应的地址

    int c = (*f)(1, 2);  // 函数调用 (*p) 类似于一个值的调用

    printf("The max value is %d \n", c);  

    f = min;    // 函数指针f指向求最小值的函数min  、

    c = (*f)(1, 2);  // 函数调用 (*p) 类似于一个值的调用
    printf("The min value is %d \n", c);  

    printf("------------------------------ End\n");  
 
    return 0;  
} 