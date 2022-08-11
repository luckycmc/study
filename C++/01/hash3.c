#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define HASHSIZE 5

typedef struct Node {
    
     char *key;
     char *value;

     struct Node *next; // 用于冲突的时候解决

}Node,*LinkList; // LinkList a 相当于 struct Node *a;

typedef LinkList HashTable[HASHSIZE];  // 相当于 struct Node *HashTable[HASHSIZE]; 结构体指针数组
//初始化hash 数组
void init(HashTable H)
{
     LinkList T; int i;
     for (i = 0; i < HASHSIZE; i++)
     {
           //创建头结点方便操作
            T = (LinkList)malloc(sizeof(Node));
            if (T == NULL)
            {
                return;
            }
            T->key = NULL;
            T->value = NULL;
            T->next = NULL;
            H[i] = T; //存放到数组中 
     }
}
//数据插入列表
int insert(HashTable H,char *key,char *value)
{
     int index;
     LinkList L,T,P;
     //获取对应的key
     index = Hash(key,HASHSIZE);
     P = H[index]; //获取当的hash表
     L = H[index]->next; //下一个节点
     while(L != NULL)
     {    
          if (equal(L->key,key))
          {    
               L->value = value;
               return 1;
          }
          P = L; 
          L = L->next;
     }
     T=(LinkList)malloc(sizeof(Node));
	 T->key=key;
	 T->value=value;
	 T->next = NULL;
	 P->next = T;

	 printf("%d,%s,%s,%s\n",index,key,value,H[index]->value);
     printf("******************************\n");
	 return 1;
}
//hash 算法
int  Hash(char *key,int mod)
{
     unsigned long  h = 0;
     while (*key)
     {
         h+=*key++;
     }
     return h % mod;
}
//判断两个key 是否相等
int equal(char *k1,char *k2)
{
     return strcmp(k1,k2) == 0?1:0;
}

//主函数
int main()
{   
    HashTable H;
	init(H);

	insert(H,"0","0");
	insert(H,"0","00000");
	insert(H,"1","1");
	insert(H,"2","2");
	insert(H,"3","3");
	insert(H,"4","4");
	insert(H,"5","5");
	insert(H,"5","55555");
	insert(H,"6","6");
	insert(H,"ab","zs100");

	insert(H,"cd","ls96");
	insert(H,"ef","ww90");
	insert(H,"0","0088000");
	insert(H,"gh","zl88");
	insert(H,"ig","qq87");
	insert(H,"mn","jb86");
	insert(H,"eue","se85");
    return 0;
}

