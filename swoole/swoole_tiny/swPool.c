#include <stdlib.h>
#include "./include/swoole.h"

/**
  Author:zpw
  频繁的对内存操作会产生什么问题
  malloc/free 函数操作内存 会产生碎片
  
  2.如何实现一个内存池
     内存池是一个组件 做出来以后提供api给其他人使用
     0.宏定义处理
     1.结构体定义出来,基础数据结构
     2.对应的函数，提供对外的api
     3.测试函数

  3.内存结构体
     1.小块内存 <4k
     2.大块内存 >4k
**/

#define MP_ALIGNMENT     32 
#define MAX_ALLOC_BLOCK  4096
//大块内存
struct mp_large_s
{
    struct mp_large_s *next;
    void *malloc;
}
//小块内存
struct mp_node_s
{
    unsigned char *start;
    unsigned char *end;

    //指向下一个节点
    struct mp_node_s *next;
    int flag;
}
//内存池的内存池
struct mp_pool_s
{
    size_t max;
    struct mp_node_s *current;
    struct mp_large_s *large;
    //定义一个柔性数据
    struct mp_pool_s head[0];
}

//定义对应的接口函数

//create pool ,init
struct mp_pool_s* mp_create_pool(size_t size)
{
     struct mp_pool_s *p;
     posix_memalign();
}
//destroy pool,
//malloc /calloc
// free 