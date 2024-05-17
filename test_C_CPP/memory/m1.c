#include <stdio.h>
//动态内存管理
// malloc ralloc realloc free 释放内存空间

//原则 谁申请水释放

#include <stdlib.h>

int main()
{
     int  *p = NULL;
     p = malloc(sizeof(int));

     if(p == NULL){
         
          printf("malloc() error\n");
          exit(1);
     }
     *p = 10;
     printf("%d\n",*p);
     free(p);
     
      *p = 123 ;  //释放后的指针在赋值就会成为野指针 不能再用了
     return 0;
}