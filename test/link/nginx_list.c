
#include <stdio.h>


typedef struct ngx_list_part_s  ngx_list_part_t;
/**
 * 链表节点  每个节点大小 = size * nelts
 * 节点元素用完后，每次就会分配一个新的节点
 */
struct ngx_list_part_s {

    void *elts;   //节点起始位置
    int nelts;    //已经使用的元素
    ngx_list_part_t *next;
};

/**
 * 链表结构
 */
typedef struct {

     ngx_list_part_t  *last;		/* 指向最新的链表节点*/
     ngx_list_part_t   part;		/* 第一个链表节点*/
     size_t            size;		/* 这个链表默认的每个元素大小 */
     int         nalloc;	        /* 每个节点part 可以支持多少个元素*/

}ngx_list_t;  //组织结构体

//创建链表
ngx_list_t *ngx_list_create(ngx_pool_t *pool,int n, size_t size)
{
    ngx_list_t *list; //结构体指针节点
    list = malloc(sizeof(ngx_list_t));

    if(list ==NULL){
         
          return NULL;
    }
    /* 分配一个链表节点的内存块。内存大小  n * size*/
    list->part.elts = malloc(sizeof(ngx_list_) * n);
    if (list->part.elts == NULL) {
        return NULL;
    }
     list->part.nelts = 0;/*使用元素的个数*/
     list->part.next  =NULL; //下一个节点
     list->last       = &last->part;  //最后一个节点的地址
     list->size       = size; //每一个元素的大小
     list->nalloc     = n;//分配多少个
}

/**
 * ngx_list_push方法可以使用一个元素，并且返回元素的指针地址
 */

void *ngx_list_push(ngx_list_t *l)
{
     void *elt;
     ngx_list_part_t *last;
     last = l->last;
     /* 如果最后一个链表节点的元素已经用完，则需要创建一个新的链表*/
     if(last->nelts == l->nalloc){
          
            /* the last part is full, allocate a new list part */
            /* 分配一块内存，存储ngx_list_part_t数据结构 */
             last = malloc(l->pool+sizeof(ngx_list_part_t));
             if(last ==NULL){
                 
                  return NULL;
             }
             /* 分配一个链表节点的内存块。内存大小  n * size*/
             last->elts = malloc(l->pool + l->nalloc * l->size);
             last->nelts = 0;
             last->next = NULL;
             /*更新链表的节点*/
             l->last->next = last;
             l->last = last;
     }
      /* 返回元素指针 */
       elt = (char *) last->elts + l->size * last->nelts;
       last->nelts++;  //尾节点加1
       return elt;    // 返回首地址
}

//主函数
int main()
{

    return 0;
}
