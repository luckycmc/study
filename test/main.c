#include <stdio.h>
#include <stdlib.h>
#include "double.h"
//主函数
int main ()
{
    printf ("*******************创建双向链表****************\n");
    line *head = NULL;
    head       = initLine(head);
    //输出创建好的链表
    display(head);
    //显示双链表的优点
    printf("链表中第 4 个节点的直接前驱是：%d\n",head->next->next->next->prior->data);
    //在表中第 3 的位置插入元素 7
    head = insertLine(head, 100, 5);
    //输出创建好的链表
    display(head);

    //表中删除元素 2
    head=delLine(head, 2);
    display(head);
    //查看数据的位置
    printf("元素 3 的位置是：%d\n",selectElem(head,3));

    //表中第 3 个节点中的数据改为存储 6
    head = amendElem(head,3,6666);
    display(head);
    return 0;
}

