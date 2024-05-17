#ifndef _DOUBLE_
#define _DOUBLE_
//节点结构体
typedef struct line
{
    struct line *prior;  //前置节点的指针
    struct line *next;   //后置节点的指针
    int     data;        // 对应的数据
}line;

//双向链表创建函数
line *initLine (line *head);
//双向链表输出函数
void display (line *head);
// 输出数据的前置节点

void displayPrev(line *head);

//给双向链表添加一个数据
line *insertLine (line *head,int data,int add);
//双链表删除指定元素
line * delLine(line * head,int data);
//查找对应的节点
//head为原双链表，elem表示被查找元素
int selectElem(line * head,int elem);

//双链表中更改指定位置节点中存储的数据，add表示更改位置
line *amendElem(line * p,int add,int newElem);

#endif //