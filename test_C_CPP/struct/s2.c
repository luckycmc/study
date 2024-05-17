#include <stdio.h>

 struct test1 {
    char a;
    int b;
 } test1;

int main() {
     printf("%lu\n", sizeof(test1));  //内存对齐输出的是8个字节
     return 0;
}