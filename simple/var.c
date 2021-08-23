#include <stdio.h>

void test(){
	static int a =1;
	a++;
	int b =1;
	b++;
	printf("a=%d, b=%d \n", a, b);
}

int main()
{
    
    int i;
    for(i=0;i<5;i++)
	{
    	test();
	}	
	return 0;
}
