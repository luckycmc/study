#include <stdio.h>
#include <stdlib.h>
#include "double.h"
//初始化双向链表
line *initLine(line *head)
{
     //创建首元素节点 链表的头指针为head
     head = (line *)malloc(sizeof(line));
     //对节点进行初始化
     head->prior = NULL;
     head->next = NULL;
     head->data = 1;
     //声明一个指向首元节点的指针，方便后期向链表中添加新创建的节点
     line *list = head;
     int i;
     for ( i = 2; i <=5; i++)
     {    
         /*(line *) 是强制转换 成对应的指针类型 mallloc()返回的是空指针类型*/
          line *body = (line *)malloc(sizeof(line)); //创建的节点新数据 并为节点开辟内存空间
          //printf("----head ----%p\n",head);
          //printf("*************************\n");
          //printf("----list ----%p\n",list);
          body->prior = NULL;
          body->next =  NULL;
          body->data = i; //当前的数据值

          //新节点与链表的的最后一个节点建立关系
          list->next = body;  //头结点的next 指向 body
          body->prior = list;  //body的前置节点指向 list也是上一个节点
          //list 指针向后移动
          list = list->next;  // list 对应的内存地址往后移动 到 list->value 的地址上
     }

     return head;  //
}
//输出链表中的数据

void display(line *head)
{
    
      line * temp = head;
      //输出链表中的数据
      while(temp != NULL)
      {
            //判断是否有后继节点
            if (temp->next == NULL)
            {
               printf("%d\n",temp->data);
            }else{
                printf("%d <-> ",temp->data);
            }
            temp = temp->next;//指针节点后移
      }
}
//输出数据的前置节点
void displayPrev(line *head)
{   
    printf("**************输出链表中的前置节点************\n");
    line * temp = head;
      //输出链表中的数据
      while(temp != NULL)
      {
            //判断是否有后继节点
            if (temp->next == NULL)
            {
               printf("%d\n",temp->data);
            }else{
                printf("%d <-> ",temp->next->prior->data);
            }
            temp = temp->next;//指针节点后移
      }
}
/****
 * 
 * int data 要插入的数据
 * int add 要插入的位置
 * */
line *insertLine (line *head,int data,int add)
{
     //新建一个域的节点
     line *temp = (line *)malloc(sizeof(line));
     /***********给对应的节点赋值 start****************/
     temp->next = NULL;
     temp->prior = NULL;
     temp->data = data;
    /***********给对应的节点赋值 end****************/
     //判断是否是链表头部
     if (add == 1)
     {
         temp->next = head;//temp 指向头部
         head->prior = temp; //上一个节点指向 temp
         head        = temp;  // head 的指向temp
     }else{
          
          //检查要查找的插入的位置
          line *body = head;
          //找到要出入的位置
          int i;
          printf("--当前数据是---%d\n",body->data);
          for ( i = 0; i < add-1; i++)
          {
              body = body->next; //指针节点往后移动
              printf("--当前数据是---%d\n",body->data);   //找到是第三个节点的记录
          }
          
          //判断是否是尾部
          if(body->next == NULL){
                 printf("****当前节点插入,当前的数据是%d\n****",body->data); 
                 body->next = temp;   //下一个节点指向新节点
                 temp->prior = body;  //新节点的前置节点指向当前的body

          }else{//中间插入节点
              /******************难点看一下  start*********************/
              body->next->prior = temp;   //下一个节点的前置节点 指向 temp 
              temp->next  = body->next;   // body->next和temp->next 指向同一个地方
              body->next  = temp;        //  衔接上一根线 当前的next 指向插入的数据
              temp->prior = body;         //插入节点的前置节点指向 body
              /******************难点看一下  end*********************/
          }
          
     }

     return head;
     
}

//删除链表的指定元素
line * delLine(line * head,int data)
{
    line *temp = head;
    //遍历链表
    while(temp){
          
           //判断当前节点是否和 data相等,若相等摘取该节点
           if (temp->data == data)
           {
               temp->prior->next=temp->next;  
               //printf("&&&&&----%d\n", temp->prior->next->data);
               temp->next->prior=temp->prior;

               free(temp); //释放当前指针的内存
               return head;
           }

        temp = temp->next;   //指针节点往后移动
    }
    printf("链表中无该数据元素");
    return head;
}

//head为原双链表，elem表示被查找元素
int selectElem(line * head,int elem)
{
      line *temp = head;
      int i = 1;
      while(temp)
      {
           if (temp->data == elem)
           {
               return i;
           }
           i++; //i 自动加1
           temp = temp->next; //节点往后移动
           
      }
      return -1;
}
//更新节点数据
line *amendElem(line * p,int add,int newElem)
{
     line *temp = p;

     int i = 0;
     for ( i = 0; i < add; i++)
     {
          temp = temp->next;//节点往后移动
     }

     temp->data = newElem;
      
    return p;
}