#include <stdio.h>
//宏定义就是一个替换
#define ZEND_CALL_FRAME_SLOT \
          (sizeof(int) * 4) * 100
          
int main()
{   
    printf("----%d\n",ZEND_CALL_FRAME_SLOT);
    return 0;
}