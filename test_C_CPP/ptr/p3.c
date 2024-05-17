#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int main() 
{
    int a[10] = {0};
    free(a);  //不能这么使用free free 只能在堆上使用编译通不过
    return 0;
}