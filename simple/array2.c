# include <stdio.h>

void change(int b[]){
	b[0] = 100;  //公用一段内存 
}

int main()
{
    
    int a[4] = {1,2,3,4};
    //讲数据a 传入 change的数组中
	change(a);
	//查看数据a[0]的值
	printf("a[0]=%d",a[0]); 
    return 0;
		
} 
