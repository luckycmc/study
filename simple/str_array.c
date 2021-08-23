# include <stdio.h>

const int MAX = 3;

int main()
{
	 int var[] = {10,100,200};
	 int i,*ptr;
	 
	 //ptr 指向元素的首地址
	 /*ptr = var;
	 
	 for(i =0; i< MAX ;i++)
	 {
	 	  printf("存储地址：var[%d] = %p\n", i, ptr );
	 	  
          printf("存储值：var[%d] = %d\n", i, *ptr );  //通过指针显示对应的数据 
 
           /* 指向下一个位置  //指针下移更具当前的数据类型 而选择移动多少个字节 
         ptr++;
	 } */
	 
	 //指针递减运算
	 /*ptr = &var[MAX];   //获取最后一个元素的地址 
	 
	 for(i = MAX ;i>0; i--)
	 {
	 	 printf("存储地址：var[%d] = %p\n", i-1, ptr );
	 	 
          printf("存储值：var[%d] = %d\n", i-1, *ptr );
          
          //指针从最大的节点向下移动
		  ptr--; 
	 } */
	 ptr = &var;  //获取指针中的第一个地址
	 i = 0;
	 while(ptr <= &var[MAX-1])
	 {
	 	  printf("存储地址：var[%d] = %p\n", i, ptr );
          printf("存储值：var[%d] = %d\n", i, *ptr );
 
          /* 指向上一个位置 */
          ptr++;  //指针向后移动 
          i++;
	 } 
	 return  0;
}
