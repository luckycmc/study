#include<stdio.h>

typedef struct TFT_DISPLAY
{
	int x;
	int y;
	void (*p)(void); //��ӡ����sss 
	int (*m)(int,int);
}; 

