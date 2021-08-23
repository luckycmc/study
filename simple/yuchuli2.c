#include <stdio.h>

#if !defined (MESSAGE)
  
   #define MESSAGE "You wish"
#endif

#define MAX(x,y) ((x) > (y) ? (x) : (y))

int main(void)
{    

	printf("Here is the  message:%s\n",MESSAGE);
	
	printf("Max between 20 and 10 is %d\n", MAX(10, 20)); 
	
	return 0;
}
