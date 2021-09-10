#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * 线程的读写锁
 * 锁只有一把  以读数据的方式枷锁---读锁。以写的方式枷锁---写锁
 * 读时共享  写时独占
 * 写锁的优先级高
 * 相对应互斥量而言 读线程多的时候回提高效率
 * 
 */
// /* 3 个线程不定时 "写" 全局资源，5 个线程不定时 "读" 同一全局资源 */
int counter;

//创建读写锁
pthread_rwlock_t rwlock;

//写线程操作空间
// 读线程操作空间