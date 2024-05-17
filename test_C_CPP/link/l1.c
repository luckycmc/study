#include <stdio.h>
#include <malloc.h>
#include <stdbool.h>
#include <stdlib.h>
/**
 * @brief
 *
 * 节点链表的定义
 *
 */
typedef struct Node
{
    int data;//数据域
    struct Node *PNext; //指向下一个域，存放下一个节点的地址
}Node,*PNode;

//创建链表
PNode create_list()
{
     int len,i;
     //獲取隊列的長度
     printf("请输入链表的长度:");
     scanf("%d",&len);
     PNode PHead = malloc(sizeof(Node));  //为头部节点分配内存空间
     PHead->PNext = NULL;  //下一个节点赋值为空
     PNode PTail  = PHead;// PTail 一直指向尾部节点是一个辅助指针
     for ( i = 0; i < len; i++)
     {
         int val;
         printf("请输入第%d个元素的值：",i+1);
         scanf("%d",&val);
         //创建新节点
        PNode PNew = malloc(sizeof(Node));
        PNew->data = val;
        PNew->PNext = NULL;
        PTail->PNext = PNew;
        PTail = PNew;  //  PTail指向节点往后移动
     }
     return PHead; //返回头部节点的指针

}
// 对链表进行遍历
void traverse(PNode pHead)
{
     PNode p=pHead->PNext;
    while(p!=NULL)
    {
        printf("%d    ",p->data);
        p=p->PNext;
    }
    printf("\n");
}
//判断链表是否为空
bool isEmpty(PNode pHead)
{
     if (pHead->PNext == NULL)
     {
         return true;
     }
     return false;
}
//获取链表的长度
int list_num(PNode pHead)
{
    int num = 0;
    PNode p = pHead->PNext;
    while (p != NULL){
        num++;
        p = p->PNext;//节点往后移动
    }
    return num;
}
//像链表出入节点
bool insert_list(PNode pHead,int val ,int pos)
{
   
}
//删除链表中的节点
//主函数
int main()
{
     PNode PHead = create_list();

     if(isEmpty(PHead)){
          printf("链表为空\n");
     }

    printf("链表的长度为：%d\n",list_num(PHead));

     traverse(PHead);

     return 0;
}
