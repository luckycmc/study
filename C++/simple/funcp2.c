#include <stdio.h>
#include <stdlib.h>

//回调函数
void populate_array(int *array,size_t arraySize,int (*getNextValue)(void))   //第三个参数是函数指针 
{   
    size_t i;
	for(i= 0;i<arraySize;i++){
		
		  array[i] = getNextRandomValue();
	} 
}
//获取随机值
int getNextRandomValue()
{
	return rand();//获取随机数 
}
//主函数 
int  main(void)
{
	int myarray[10];
	
	populate_array(myarray,10,getNextRandomValue);
	int i;
	for(i = 0; i < 10; i++) {
        printf("%d ", myarray[i]);
    }
    printf("\n");
    return 0;
} 
