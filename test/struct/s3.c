#include <stdio.h>
/**
 * @brief 
 * 采用的是紧凑发内存为5个字节 也就是实际的内存大小
 */
struct __attribute__((packed)) test2  {
    char a;
    int b;
 } test2;

int main() {
     printf("%lu\n", sizeof(test2));
     return 0;
}