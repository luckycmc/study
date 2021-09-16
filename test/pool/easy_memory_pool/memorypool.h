#ifndef _MEMORYPOOL_H_
#define _MEMORYPOOL_H_
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
/********
 * 内存池管理
 * */
typedef struct Memorypool
{
    int mempool_size;

    int memory_size;//block_size + sizeof(manager information) = mem_size
    int block_size;

    int total_memory_cnt;
    int used_memory_cnt;
    int auto_extend;

    struct Memory* free_list;
    struct Memory* used_list;
}memorypool;
//内存
typedef struct Memory
{
    int memory_size;         //内存的大小
    int block_size;          // 块大小
    struct Memory* prev;    
    struct Memory* next;

    struct Memorypool* mp;
    char* block;
}memory;
// 内存池的创建
memorypool* memorypool_create(int block_request_size, int memory_init_quantity, int memory_extend);
// 内存的申请
char* memory_malloc(memorypool* mp,int data_size);
//获取内存池的信息
int memorypool_info_get(memorypool* mp);
// 内存的释放
int memory_free(char** p_bk);
//内存的销毁
int memorypool_destroy(memorypool* mp);
//内存块的创建
memory* memory_creat(int memory_size, memorypool* mp);


#endif