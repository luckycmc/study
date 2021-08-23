#ifndef _QUEUE_
#define _QUEUE_

//链表中的结构体
    typedef struct QNode {
          int data;
          struct QNode *next;
    }QNode;

//链表的初始化
QNode * initQueue();
//数据入队列
QNode* enQueue(QNode * rear,int data);
//数据初队列     
QNode *DeQueue(QNode * top,QNode * rear);

#endif // !_QUEUE_
