#include<stdio.h>
#include<pthread.h>
#include <stdlib.h>

int s;
void *func(void *arg){
    printf("------\n");
    int i;
    for (i = 0; i < 10000; i++)
    {
        s++;
        printf("s is %d\n",s);
    }  
}

int main(){
    pthread_t th1;
    pthread_t th2;
    printf("main s is %d\n",s);
    //创建两个线程 函数入口 
    pthread_create(&th1,NULL,func,NULL);
    pthread_create(&th2,NULL,func,NULL);

    pthread_join(th1,NULL);
    pthread_join(th2,NULL);

    printf("s is %d\n",s);
    return 0;
}