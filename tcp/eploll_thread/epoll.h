#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
/**
 * 对应线程执行的函数
 */
void *ThreadFunc()
{
    static int count = 1;
    printf ("Create thread %d\n", count);
    //对线程进行回收  
    pthread_detach(pthread_self()); //标记为DETACHED状态，完成后释放自己占用的资源。
    count++;
}
//主函数
main(void)
{
    int     err;
    pthread_t tid; //  pthread_t用于声明线程ID。
    while (1)
    {
           err= pthread_create(&tid, NULL, ThreadFunc, NULL);
           if(err != 0){
               printf("can't create thread: %s\n",strerror(err)); //strerror 将单纯的错误标号转为字符串描述，方便用户查找错误
           break;
           }
          usleep(2000);
    }
    //  gcc -o thread ./thread.c -lpthread 编译线程对应的服务器
}
