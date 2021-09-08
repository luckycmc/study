#include <stdio.h>


int enQueue(int *a,int rear,int data)
{
    a[rear] = data;  //赋值对应的数组
    rear++;         //尾部指针往后移动
    return rear;
}

//出队列
void deQueue(int *a,int front,int rear)
{
     //从队列中取出数据
     while(front!=rear)
     {
         printf("出队列元素:%d\n",a[front]);
         front++;
     }
}
//主函数
int main()
{
     int a[100];//数组的长度为100；
     int front,rear;//头和尾

     front = rear =0;
     //入队
    rear=enQueue(a, rear, 1);
    rear=enQueue(a, rear, 2);
    rear=enQueue(a, rear, 3);
    rear=enQueue(a, rear, 4);
    //出队
    deQueue(a, front, rear);
    return 0;
}