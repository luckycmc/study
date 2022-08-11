#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define HASHSIZE 5
#define name_Max 10
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
     while(L != NULL)  // 等于 null 的时候说明为空节点了
     {    
          if (equal(L->key,key)) //两个值相等的时候直接退出了
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
//删除hash
//修改
//查找
//展现所有的hash
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
//显示数据
void showData()
{
    printf("show\n");
}
void menu()
{
    printf("***************************\n");
    printf("***  1.add    2.del     ***\n");
    printf("***  3.serch  (modify)  ***\n");
    printf("***  4.sort   5.show    ***\n");
    printf("***  0.exit   6.clear   ***\n");
    printf("***************************\n");
}
//主函数
int main()
{   
    
    HashTable H;
	init(H); //初始化hash表 
    int input = 0; char key[name_Max];char value[name_Max];     
    do{
       menu();
        printf("请选择->");
       scanf("%d",&input);
       
       switch(input)
       {
          case 1: 
                printf("请选择 key 和 value\n");
                scanf("%c %c",&key,&value);
                insert(H,key,value);
                break;
           case 2: 
               break;
           case 3: 
               showData();
               break;
           case 0:
               printf("退出hash表");
               break;
           default:
              break;
       }
    }while(input);
	/*insert(H,"0","0");
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
	insert(H,"eue","se85");*/
    return 0;
}

