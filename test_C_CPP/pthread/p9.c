#include <stdio.h>
/**
  条件变量
*/

#include <pthread.h> 
#include <unistd.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <signal.h> 

// 定义条件变量cond_product 
    pthread_cond_t cond_product; 

// 定义条件变量cond_consume 
    pthread_cond_t cond_consume; 

// 定义线程互斥锁lock 
    pthread_mutex_t lock; 

//初始化函数 
void init_work(void) 
{ 
// 条件变量的初始化 
    pthread_cond_init(&cond_product, NULL); 
    pthread_cond_init(&cond_consume, NULL); 

// 线程锁的初始化 
    pthread_mutex_init(&lock, NULL); 
} 

//生产线程，每次生产一个产品 
void* handle_product(void *arg){ 
    int i; 
    int* product = NULL; 
    product = (int *) arg; 
    for(i=1; i<50; i++) { 
        pthread_mutex_lock(&lock); //生产进程上锁，是消费进程无法改变商品个数 
        if (*product >= 4) { 
// 仓库已满，应该阻塞式等待 
            printf("\033[43m仓库已满, 暂停生产...\033[0m\n"); 
            pthread_cond_wait(&cond_product, &lock); 
        } 
        printf("生产中....\n"); 
        sleep(2); 
        printf("生产成功....\n"); 
        *product+=1; 
        pthread_cond_signal(&cond_consume);//发出信号，条件已经满足 
        printf("\033[32m生产一个产品,生产%d次，仓库中剩余%d个\033[0m\n",i,*product); 
        printf ("发信号--->生产成功\n"); 
        pthread_mutex_unlock(&lock);//生产进程解锁 
        usleep(50000); 
    } 
        return NULL; 
} 

//消费线程，每次消费两个产品，消费6次间歇 
void* handle_consume(void *arg){ 
    int i; 
    int* product = NULL; 
    product = (int *)arg; 
        for (i=1; i<50; i++) { 
        pthread_mutex_lock(&lock); 
        if (*product <= 1) //消费线程每次消费2个，故总产品数量小于1即阻塞 
        { 
/* 阻塞式等待 */ 
            printf("\033[43m缺货中，请等待...\033[0m\n"); 
            pthread_cond_wait(&cond_consume, &lock); 
        }             
 /* 消费产品，每次从仓库取出两个产品 */ 
        printf("消费中...\n"); 
         sleep(2); 
        *product-=2; 
        printf("消费完成...\n"); 
        printf("\033[31m消费两个产品,共消费%d次，仓库剩余%d个\033[0m\n",i,*product); 
        pthread_cond_signal(&cond_product); 
        printf ("发信号---> 已消费\n"); 
        pthread_mutex_unlock(&lock); 
        usleep(30000); 
        if (i%6 == 0){ //消费间歇 
            sleep(9); 
        } 
    } 
        return NULL; 
} 

int main() 
{ 
    pthread_t th_product, th_consume; //定义线程号 
    int ret; 
    int intrinsic = 3; 
//初始化所有变量 
    init_work(); 
//创建进程并传递相关参数 
    ret = pthread_create(&th_product, 0, handle_product, &intrinsic); 
    if (ret != 0) { 
        perror("创建生产线程失败\n"); 
        exit(1); 
    } 

    ret = pthread_create(&th_consume, 0, handle_consume, &intrinsic); 
    if (ret != 0) { 
        perror("创建消费线程失败\n"); 
        exit(1); 
    } 

    pthread_join(th_product, 0);//回收生产线程 
    pthread_join(th_consume, 0);//回收消费线程 

    return 0; 
}