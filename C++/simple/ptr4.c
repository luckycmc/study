#include <stdio.h>

/**
  指向指针的指针 即使为二级指针 
*/ 

int  main()
{
	 int V;
	 
	 int *Pt1; //一级指针 
	 
	 int **Pt2;  // 二级指针
	 
	 
	 V =100;
	 
	 //获取 V的地址
	 Pt1 = &V;
	 
	 //使用 & 运算符 获取Pt1 的地址
	 Pt2 = &Pt1;  //PT2即使指向指针的指针 
	 
	  /* 使用 pptr 获取值 */
    printf("var = %d\n", V );
    printf("Pt1 = %p\n", Pt1 );
    printf("*Pt1 = %d\n", *Pt1 );
    printf("Pt2 = %p\n", Pt2 );
    printf("**Pt2 = %d\n", **Pt2);
    return 0; 
} 
