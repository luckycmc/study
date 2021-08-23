#include <stdio.h>

#define MAX(x,y)((x)>(y)?(x):(y))

#define squre(x)((x)*(x))
int main()
{
	printf("Max between 20 and 10 is %d\n", MAX(10, 20));
	
	printf("sq is %d\n",squre(100));
    return 0;
}
