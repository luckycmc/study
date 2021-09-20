
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>



#define MP_ALIGNMENT       		32
#define MP_PAGE_SIZE			4096
#define MP_MAX_ALLOC_FROM_POOL	(MP_PAGE_SIZE-1)

#define mp_align(n, alignment) (((n)+(alignment-1)) & ~(alignment-1))
#define mp_align_ptr(p, alignment) (void *)((((size_t)p)+(alignment-1)) & ~(alignment-1))

//大块内存
struct mp_large_s 
{
	struct mp_large_s *next; //下一个大节点的指针
	void *alloc; //对应的数据区
};
// 内存节点
struct mp_node_s 
{

	unsigned char *last;  // 上一个位置
	unsigned char *end;   // 结束的位置
	
	struct mp_node_s *next; // 对应的下一一个节点

	size_t failed; //失败节点数
};
//内存池
struct mp_pool_s {

	size_t max; //最大数量

	struct mp_node_s *current;  // 当前节点
	struct mp_large_s *large;  //大块内存节点

	struct mp_node_s head[0];  // 头节点

};

struct mp_pool_s *mp_create_pool(size_t size);        //内存池的创建
void mp_destory_pool(struct mp_pool_s *pool);         // 内存池的销毁
void *mp_alloc(struct mp_pool_s *pool, size_t size);
void *mp_nalloc(struct mp_pool_s *pool, size_t size);
void *mp_calloc(struct mp_pool_s *pool, size_t size);
void mp_free(struct mp_pool_s *pool, void *p);  // 内存池的释放

/**
 * @内存池的创建
 * 
 * @param size 
 * @return struct mp_pool_s* 
 */
struct mp_pool_s *mp_create_pool(size_t size) 
{
    // posix_memalign为您提供了保证具有所请求对齐的内存块。
	struct mp_pool_s *p;
	//内存对齐
	int ret = posix_memalign((void **)&p, MP_ALIGNMENT, size + sizeof(struct mp_pool_s) + sizeof(struct mp_node_s));
	if (ret) {
		return NULL;
	}
	
	p->max = (size < MP_MAX_ALLOC_FROM_POOL) ? size : MP_MAX_ALLOC_FROM_POOL; //内存池的最大个数
	p->current = p->head; //
	p->large = NULL;
    //下一个节点开始的地方
	p->head->last = (unsigned char *)p + sizeof(struct mp_pool_s) + sizeof(struct mp_node_s);
	p->head->end = p->head->last + size; // 尾部节点

	p->head->failed = 0; // 失败的次数

	return p;

}
/**
 * @brief  内存池的销毁
 * 
 * @param pool 
 */
void mp_destory_pool(struct mp_pool_s *pool) 
{

	struct mp_node_s *h, *n;
	struct mp_large_s *l;
    //销毁大块内存
	for (l = pool->large; l; l = l->next) {
		if (l->alloc) {
			free(l->alloc);
		}
	}

	h = pool->head->next;  //小块内存
    //释放小块内存
	while (h) {
		n = h->next;
		free(h);
		h = n;
	}

	free(pool);

}
/**
 * 重置内存池
 * @brief 
 * 
 * @param pool 
 */
void mp_reset_pool(struct mp_pool_s *pool) {

	struct mp_node_s *h;
	struct mp_large_s *l;
    // 释放内存节点
	for (l = pool->large; l; l = l->next) {

		
		if (l->alloc) {
			printf("memeory is %p\n",l->alloc);
			free(l->alloc);  //释放所有的节点
		}
	}

	pool->large = NULL;

	for (h = pool->head; h; h = h->next) {
		h->last = (unsigned char *)h + sizeof(struct mp_node_s);//内存节点扩大一倍
		printf("memeory is %p\n",h->last);
	}

}
/**
 * 创建内存池快
 * 
 * @param pool 
 * @param size 
 * @return void* 
 */
static void *mp_alloc_block(struct mp_pool_s *pool, size_t size) {

	unsigned char *m;
	struct mp_node_s *h = pool->head;
	size_t psize = (size_t)(h->end - (unsigned char *)h);
	
	int ret = posix_memalign((void **)&m, MP_ALIGNMENT, psize);
	if (ret) return NULL;

	struct mp_node_s *p, *new_node, *current;
	new_node = (struct mp_node_s*)m;

	new_node->end = m + psize;
	new_node->next = NULL;
	new_node->failed = 0;

	m += sizeof(struct mp_node_s);
	m = mp_align_ptr(m, MP_ALIGNMENT);
	new_node->last = m + size;

	current = pool->current;

	for (p = current; p->next; p = p->next) {
		if (p->failed++ > 4) {
			current = p->next;
		}
	}
	p->next = new_node;

	pool->current = current ? current : new_node;

	return m;

}
/**
 * @brief 创建大块内存
 * 
 * @param pool 
 * @param size 
 * @return void* 
 */
static void *mp_alloc_large(struct mp_pool_s *pool, size_t size) {

	void *p = malloc(size);
	if (p == NULL) return NULL;

	size_t n = 0;
	struct mp_large_s *large;
	//查找好对应的大块内存
	for (large = pool->large; large; large = large->next) {
		if (large->alloc == NULL) {
			large->alloc = p;
			return p;
		}
		if (n ++ > 3) break; //三次没有 退出
	}

	large = mp_alloc(pool, sizeof(struct mp_large_s));  //申请内存
	if (large == NULL) {
		free(p);
		return NULL;
	}

	large->alloc = p;  // 大节点对应的内存区域
	large->next = pool->large;//下一个节点
	pool->large = large;    //大块内存

	return p;
}
// 内存对齐
void *mp_memalign(struct mp_pool_s *pool, size_t size, size_t alignment) {

	void *p;
	
	int ret = posix_memalign(&p, alignment, size);
	if (ret) {
		return NULL;
	}

	struct mp_large_s *large = mp_alloc(pool, sizeof(struct mp_large_s));
	if (large == NULL) {
		free(p);
		return NULL;
	}

	large->alloc = p;
	large->next = pool->large;
	pool->large = large;

	return p;
}



/***
 * 创建内存节点
 * */
void *mp_alloc(struct mp_pool_s *pool, size_t size) {

	unsigned char *m;
	struct mp_node_s *p;

	if (size <= pool->max)  //小内存 
	{

		p = pool->current;
		do {
			
			m = mp_align_ptr(p->last, MP_ALIGNMENT);
			if ((size_t)(p->end - m) >= size) {
				p->last = m + size;
				return m;
			}
			p = p->next;
		} while (p);

		return mp_alloc_block(pool, size);
	}

	return mp_alloc_large(pool, size); //走大内存
	
}

//内存区域的创建
void *mp_nalloc(struct mp_pool_s *pool, size_t size) {

	unsigned char *m;
	struct mp_node_s *p;

	if (size <= pool->max) {
		p = pool->current;

		do {
			m = p->last;
			if ((size_t)(p->end - m) >= size) {
				p->last = m+size;
				return m;
			}
			p = p->next;
		} while (p);

		return mp_alloc_block(pool, size);
	}

	return mp_alloc_large(pool, size);
	
}
//调用malloc并初始化为0
void *mp_calloc(struct mp_pool_s *pool, size_t size) {

	void *p = mp_alloc(pool, size);
	if (p) {
		memset(p, 0, size); //调用malloc并初始化为0
	}

	return p;
	
}
/**
 * @brief 内存池的释放
 * 
 * @param pool 
 * @param p 
 */
void mp_free(struct mp_pool_s *pool, void *p) {

	struct mp_large_s *l; //拿到节点遍历所有的节点

	for (l = pool->large; l; l = l->next) {
		if (p == l->alloc) {
			free(l->alloc);
			l->alloc = NULL;

			return ;
		}
	}
	
}


int main(int argc, char *argv[]) {

	int size = 1 << 12;
    
	struct mp_pool_s *p = mp_create_pool(size); //申请内存池

	int i = 0;
	for (i = 0;i < 10;i ++) {

		void *mp = mp_alloc(p, 512);
//		mp_free(mp);
	}

	//printf("mp_create_pool: %ld\n", p->max);
	printf("mp_align(123, 32): %d, mp_align(17, 32): %d\n", mp_align(24, 32), mp_align(17, 32));
	//printf("mp_align_ptr(p->current, 32): %lx, p->current: %lx, mp_align(p->large, 32): %lx, p->large: %lx\n", mp_align_ptr(p->current, 32), p->current, mp_align_ptr(p->large, 32), p->large);

	int j = 0;
	for (i = 0;i < 5;i ++) {

		char *pp = mp_calloc(p, 32);
		for (j = 0;j < 32;j ++) {
			if (pp[j]) {
				printf("calloc wrong\n");
			}
			//sleep(1);
			printf("calloc success\n");
		}
	}

	//printf("mp_reset_pool\n");

	for (i = 0;i < 5;i ++) {
		void *l = mp_alloc(p, 8192);
		mp_free(p, l);
	}

	mp_reset_pool(p); //重置内存池

	//printf("mp_destory_pool\n");
	for (i = 0;i < 58;i ++) {
		mp_alloc(p, 256);
	}

	mp_destory_pool(p);

	return 0;

}



