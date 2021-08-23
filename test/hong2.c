#include <stdio.h>

int main()
{
  
    #if _WIN32
         system("color 0c");
         printf("http://c.biancheng.net\n");
    #elif __linux__
         printf("\033[22;31mhttp://c.biancheng.net\n\033[22;30m");
    #else
         printf("http://c.biancheng.net\n");          
    #endif
    
    return 0;
}
