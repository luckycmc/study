#include <stdio.h>
#include <stdlib.h>

/**************************************************
dynamicArrayOneDimensional.c
输入N个数，求平均数
**************************************************/
main()
{
  int *p = NULL, n, i, sum;
  printf("Please enter array size:");
  scanf("%d", &n);

  /* 申请地址n个长度为sizeof(int)的连续存储空间 */
  p = (int *)malloc(n * sizeof(int));

  if(p == NULL)
  {
      printf("No enough memory!\n");
      exit(0);
  }

  printf("Please enter the score:");
  for(i = 0; i<n; i++)
  {
      scanf("%d", p+i);
  }

  sum = 0;
  for(i = 0; i<n; i++)
  {
      sum = sum + *(p + i);
  }

  printf("aver = %d\n", sum/n);
  free(p);

}