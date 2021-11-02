#include <stdio.h>
#include <stdlib.h>
#include "t.h"


/**
 * 动态申请跳表节点.
 * 
 * @return NULL:内存申请失败
 *        !NULL:节点创建成功
 */
static skip_list_node* skip_list_node_creat(int level, int key, int value)
{
	struct skip_list_node *node = NULL;
	
	/* 节点空间大小 为节点数据大小+ level层索引所占用的大小 */
	node = malloc(sizeof(*node) + level * sizeof(node));
	if (node == NULL)
		return NULL;
	
	/* 清空申请空间 清零*/
	memset(node, 0, sizeof(*node) + level * sizeof(node));
	node->key = key;          //对应的key
	node->value = value;      //对应的value
	node->max_level = level;  //层级 
	 
	return node;
}


/**
 * 创建跳表头节点.
 * 
 * @param max_level:跳表最大层数
 * @return NULL:创建失败
 *        !NULL:创建成功
 */
struct skip_list* skip_list_creat(int max_level)
{
	struct skip_list *list = NULL;
	
	list = malloc(sizeof(*list));
	if (list == NULL)
		return NULL;
	
	list->level = 1;  //成高为1
	list->count = 0;  //个数为0
	list->head = skip_list_node_creat(max_level, 0, 0);
	if (list->head == NULL)
	{
		SKIP_LIST_FREE(list);
		return NULL;
	}
	
	return list;
}


/**
 * 随机产生插入元素的索引层数,随机产生的.
 * 
 * @param list:跳表
 * @return 节点索引层数
 *        
 */
static int skip_list_level(struct skip_list *list)
{
	int i = 0, level = 1; /*索引层数至少为1,所以从1开始*/
	
	for (i=1; i<list->head->max_level; i++)
	{
		if ((rand() % 2) == 1)
		{
			level++;
		}
	}
	return level;
}

/**
 * 插入跳表节点.
 * 
 * @param list:跳表
 * @param key:
 * @param value:
 * @return -1:跳表为空
 *         -2:空间分配失败
 *         -3:key已经存在
 *          0:插入成功
 */
int skip_list_insert(struct skip_list *list, int key, int value)
{
	struct skip_list_node **update = NULL; /*用来更新每层索引指针，存放插入位置的前驱各层节点索引*/ 
	struct skip_list_node *cur = NULL;
	struct skip_list_node *prev = NULL;
	struct skip_list_node *insert = NULL;
	int i = 0, level = 0;
	
	if (list == NULL)
		return -1;
	
	/*申请update空间用于保存每层的索引指针*/
	update = (struct skip_list_node **)malloc(sizeof(list->head->max_level * sizeof(struct skip_list_node *)));
	if (update == NULL){
         return -2;
    }
		
	/*逐层查询,查找插入位置的前驱各层节点索引
	 *update[0] 存放第一层的插入位置前驱节点，update[0]->next[0]表示插入位置的前驱节点的下一节点(update[0]->next[0])的第一层索引值
	 *update[1] 存放第二层的插入位置前驱节点，update[1]->next[1]表示插入位置的前驱节点的下一节点(update[1]->next[0])的第二层索引值
	 *update[n] 存放第一层的插入位置前驱节点，update[n]->next[n]表示插入位置的前驱节点的下一节点(update[n]->next[0])的第n层索引值
	 */
	prev = list->head; /*从第一个节点开始的最上层开始找*/
	i = list->level - 1;  
	for(; i>=0; i--)
	{
		/* 各层每个节点的下一个节点不为空 && 下个节点的key小于要插入的key */
		while ( ((cur = prev->next[i]) != NULL) && (cur->key < key) )
		{
			prev = cur; /* 向后移动 */
		}
		update[i] = prev; /* 各层要插入节点的前驱节点 */
	}
	
	/* 当前key已经存在,返回错误 */
	if ((cur != NULL) && (cur->key == key))
	{
		return -3;
	}
	
	/*获取插入元素的随机层数,并更新跳表的最大层数*/
	level = skip_list_level(list);
	/*创建当前节点*/
	insert = skip_list_node_creat(level, key, value);
	/*根据最大索引层数,更新插入节点的前驱节点,前面已经更新到了[0] - [(list->level-1)]*/
	if (level > list->level)
	{
		for (i=list->level; i<level; i++)
		{
			update[i] = list->head;/*这部分为多新增的索引层,所以前驱节点默认为头结点*/
		}
		list->level = level;/*更新跳表的最大索引层数*/
	}
	
	/*逐层更新节点的指针*/
	for (i=0; i<level; i++)
	{
		insert->next[i] = update[i]->next[i];
		update[i]->next[i] = insert;
	}
	
	/*节点数目加1*/
	list->num++;
	
	return 0;
}

