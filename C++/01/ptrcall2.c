#include<stdio.h>

typedef struct TFT_DISPLAY
{
	int x;
	int y;
	void (*p)(void); //´òÓ¡º¯Êýsss 
	int (*m)(int,int);
}; 

