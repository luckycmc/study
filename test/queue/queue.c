#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

//链表的初始化
QNode *initQueue()
{
    QNode *queue = (QNode *)malloc(sizeof(QNode));
    queue->next = NULL;
    return queue;
}

//入队列
QNode *enQueue(QNode *rear,int data)
{
      QNode *enElem = (QNode *)malloc(sizeof(QNode));
      enElem->data = data;
      enElem->next = NULL;
      printf("数据入队列%d\n",data);
      //printf("数据入队列%p\n",rear);
      //使用尾插法向链队列中添加数据元素
      rear->next = enElem;
      rear       = enElem; //尾部指针往后移动
      return rear;
}

//出队列
QNode* DeQueue(QNode * top,QNode * rear){
    //top 代表指针的头结点
    if (top->next==NULL) {
        printf("\n队列为空");
        return rear;
    }
    QNode * p = top->next; //指针p指向top ->next 的下一个节点的数据
    // top->next  代表第一个节点
    printf("%d ",p->data);
    //头部节点往后移动
    top->next = p->next;
    //判断是否是尾部节点
    if (rear==p) {
        rear = top;
    }
    free(p);
    return rear;
}

int main() {
    // 
    QNode * queue,*top,*rear;
    queue = top = rear=initQueue();//创建头结点
    //向链队列中添加结点，使用尾插法添加的同时，队尾指针需要指向链表的最后一个元素
    int i =0;
    int num ;
    printf("请输入队列的长度:");
    
    scanf("%d",&num);
    while(1){
         if (i>=num)
         {
             break;
         }
         rear = enQueue(rear, i);
         i++;
    }
    
   
    //入队完成，所有数据元素开始出队列
    while(1){
         rear = DeQueue(top, rear);
         if (top->next == NULL)
         {
             break;
         }
         
    }
   
    printf("\n");
    return 0;

}