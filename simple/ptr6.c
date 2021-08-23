#include <stdio.h>

/**
  传递数组指针 
*/ 
double getAverage(int *arr,int size);


int main()
{
	/* 带有 5 个元素的整型数组  */
   int balance[5] = {1000, 2, 3, 17, 50};
   double avg;
 
   /* 传递一个指向数组的指针作为参数 */
   avg = getAverage( balance, 5 ) ;
 
   /* 输出返回值  */
   printf("Average value is: %f\n", avg );
    
   return 0;
} 

double getAverage(int *arr, int size)
{
  int    i, sum = 0;       
  double avg;          
  printf("***arr%p\n",&arr);
  for (i = 0; i < size; ++i)
  {  
    printf("每个元素的地址%d\n",arr[i]);
    sum += arr[i];
  }
 
  avg = (double)sum / size;
 
  return avg;
}
