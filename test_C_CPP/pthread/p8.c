#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
/*
  try 是锁尝试枷锁  
  读写锁  读共享 写优先 写的优先级高
  锁只有一把以读的方式给数据枷锁---读锁 以写的方式给数据枷锁 ---写锁
  相对于互斥锁 而言,当线程多的时候,提高访问效率
*/
/**
   pthread_rwlock_t rwlock;
   pthread_rwlock_init(&rwlock,NUll);
   pthread_rwlock_rdlock(&rwlock);
   pthread_rwlock_wrlock(&rwlock);
   pthread_rwlock_unlocl(&rwlock);
   pthread_rwlock_destroy(&rwlock);
*/
//创建读写锁
pthread_rwlock_t rwlock;

void *pthread_one(void *arg);
void *pthread_two(void *arg);

//主函数 
int main()
{
      int i = 0;
      pthread_t id[2]; //创建线程id


      //读写锁初始化
      pthread_rwlock_init(&rwlock, NULL);

      //创建写线程
       pthread_create(&id[0], NULL, pthread_one, NULL);
       //sleep(1);
       //创建第二个线程
       pthread_create(&id[1], NULL, pthread_two, NULL);
       //回收线程
       for(; i<2; i++){

           pthread_join(id[i], NULL);
       }

       /*销毁线程的读写锁*/
      pthread_rwlock_destroy(&rwlock);

      return 1;     
}

//写锁
void *pthread_one(void *arg)
{  
   while(1){
    pthread_rwlock_wrlock(&rwlock);      
           //添加写锁
    //pthread_rwlock_wrlock(&rwlock);

        puts("rdlock locked first, pthread one!");

        sleep(2);

        puts("after sleep 2s");
    
    pthread_rwlock_unlock(&rwlock);
   }
   

}
//读锁
void *pthread_two(void *arg)
{    
    while(1){
          pthread_rwlock_rdlock(&rwlock);

             puts("got the rdlock, pthread two!");

          pthread_rwlock_unlock(&rwlock);
    }
    
}

//  gcc p8.c -o p8 -lpthread 线程的编译