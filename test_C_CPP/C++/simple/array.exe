#include <stdio.h>

int main()
{

    
    // 遍历一个数组
	// 定义一个int类型的数组
	 int a[4] = {1, 2, 3, 4};
	
	 int *p = a;
	 
	 int i;
	 //for (i = 0; i < 4; i++) {
	 //    printf("a[%d] = %d \n", i, a[i]);
	 //} 
	 printf("利用指针显示数据\n");
	 //用指针遍历数组
	 for(i=0 ;i<4;i++){
	 	
	 	//利用指针*取数数据的值
		 int value = *(p++);  // p+1 由于是int 所以指针移动 4个为 就是 4个字节 
		 printf("a[%d] = %d \n", i, value);
	 } 
	return 0;
}
