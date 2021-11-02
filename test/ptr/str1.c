#include <stdio.h>
#include <string.h>
int main()
{   
    char str1[] = "I am an phper I have six year";
    int length = strlen(str1);
    int i;
    for ( i = 0; i <length ; i++)
    {
         printf("%p\n",&str1[i]);
    }
    return 0;
}