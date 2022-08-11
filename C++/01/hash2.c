// https://blog.csdn.net/qq_49101164/article/details/116920942 学习地址来源

// 采用链表的方式 遇到冲突后用链表实现
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/************hash 表中的元素结构体 start**********/
typedef struct
{
   char *key; //关键词
   char *value; //
}Element;

//数据元素单链表
typedef struct Node {
     
       Element student;//数据元素
       struct Node *next; //next 指针
}Node;
//hashTable 数据表
typedef struct{
    Node *head; // 数据存放
    int size; // 大小当前表中的大小
    int count;//元素的个数
}HashTable;
/************hash 表中的元素结构体 end**********/

// 初始化hashtbale
HashTable *inittable(int tablesize) 
{
    HashTable *table;
    table = (HashTable *)malloc(sizeof(HashTable));
    if (table==NULL) 
    {   
        printf("malloc failure\n");
        return NULL;
    }
    table->size = tablesize;
    //分配和初始化单链表中的头结点
    table->head = (Node *)malloc((table->size) * sizeof(Node));
    if (table->head == NULL)
    {   
        printf("malloc failure\n");
        return NULL;
    }
    memset(table->head,0,sizeof(Node) * (table->size));
    table->count =0;
    return table;
}
//hash 算法
int Hash(HashTable *table,char *key)
{
    int sum = 0; int i;
    for ( i = 0; i < strlen(key); i++ )
    {
        sum += key[i];
    }
    return sum % table->size;
}
//在hash表中查找关键字
Node *find(HashTable *table,char *key)
{
    //首先获取key
    int ii = Hash(table,key);
    Node *node = table->head[ii].next;
    //遍历链表
    while((node != NULL) && (node->student.key != key))
    {
        node = node->next; //链表节点后移
    }
    return node;
}
// 把数据插入到hash表中
int insert(HashTable *table,char *key,char *value)
{
      //从hash 表中查找对应的元素
      Node *findNode = find(table,key);
      if (findNode != NULL)
      {
           printf("该几点元素已经存在!\n");
           return 0;
      }
      int ii = Hash(table,key);
      //创建一个新的节点
      Node *node = (Node *)malloc(sizeof(Node));
      if (node == NULL)
      {
           printf("applicatio memmory!\n");
           return 0;
      }
      node->student.key = key;
      node->student.value = value;
      
      //吧节点追加到头结点的后面
      node->next = table->head[ii].next;
      table->head[ii].next = node;
      table->count++;
      return 1;
}
//遍历hash表
void show(HashTable *table)
{
     int i;
     for ( i = 0; i <table->size; i++ )
     {
           Node *node = table->head[i].next;
           while (node)
           {
              printf("%s-%s\n", node->student.key, node->student.value);
              node = node->next; //节点后移
           }
           printf("-----\n");
     }
      printf("&&&&&&\n");
}
int main()
{   
    HashTable *table = inittable(5); //初始化数据表

    insert(table,"zpw","zpw 666");
    insert(table,"box","box 666");
    insert(table,"kill","kill 666");
     //打印数据表
    show(table);
    return 0;
}