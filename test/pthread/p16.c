#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

/****
 输出结果是主线程和子线程交叉的,因为产生的线程切换 导致数据输出不一样
 * */
//线程的执行函数
void *tfn()
{
    srand(time(NULL));
    while (1){
        printf("hello");
        sleep(rand()%3); /*模拟长时间操作共享资源，导致 cpu 易主，产生与时间有关的错误*/
        printf("world\n");
        sleep(rand()%3);
    }
    return NULL;
}
//主函数
int main()
{
     pthread_t tid;
     srand(time(NULL));
     //创建线程
     pthread_create(&tid,NULL,tfn,NULL);

     while(1){
         printf("HELLO ");
         sleep(rand() % 3);
         printf("WORLD\n");
         sleep(rand() % 3);
     }
     pthread_join(tid, NULL);
     return 0;
}

