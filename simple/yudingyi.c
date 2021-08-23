#include <stdio.h>

# define message_for(a,b)\
   printf(#a " and " #b ": We love you!\n"); 
   
int main()
{
 	/*printf("File :%s\n", __FILE__ );  //打印前 文件的位置 
    printf("Date :%s\n", __DATE__ );    // 当前系统的日期 
    printf("Time :%s\n", __TIME__ );    // 当前系统的时间 
    printf("Line :%d\n", __LINE__ );    // 文件的行数 
    printf("ANSI :%d\n", __STDC__ );*/ 
    
    //调用的预定于的处理器
	message_for(Carole, Debra); 
	return 0;
}
