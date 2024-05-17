#include <stdio.h>
#include <time.h>

//声明一个函数
void getSeconds(unsigned long *par); 
/**
 传指针给函数 
*/
int main()
{   

	unsigned long sec;
    getSeconds( &sec );  //修改指针的指向位置 得到相应的数据值 
    /* 输出实际值 */
    printf("Number of seconds: %ld\n", sec );
    return 0;
} 

void getSeconds(unsigned long *par)
{
     /* 获取当前的秒数 */
   *par = time( NULL );
   return;
} 
