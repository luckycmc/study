#include <stdio.h>
/**
 * C语言回调函数
 * */

int Callback_1(int x) // Callback Function 1
{
    printf("Hello, this is Callback_1: x = %d \n", x);
    return 0;
}

int Callback_2(int x) // Callback Function 2
{
    printf("Hello, this is Callback_2: x = %d \n", x);
    return 0;
}

int Callback_3(int x) // Callback Function 3
{
    printf("Hello, this is Callback_3: x = %d\n ", x);
    return 0;
}

int Handle(int y, int (*Callback)(int))
{
    printf("Entering Handle Function. \n");
    Callback(y);
    printf("Leaving Handle Function. \n");
}

int main()
{
    int a = 2;
    int b = 4;
    int c = 6;
    printf("Entering Main Function. \n");
    Handle(a, Callback_1);
    Handle(b, Callback_2);
    Handle(c, Callback_3);
    printf("Leaving Main Function. \n");
    return 0;
}