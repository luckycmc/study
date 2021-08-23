#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "book.h"

void menu()
{   
    AddrBook addr_book;
    //初始化
    AddrBookInit(&addr_book); //初始化 addr_book 地址指向
   // AddrBookLoad(&addr_book);
    int flag = 0;
    while (1){
            printf("============ 1.打印通讯录=============\n");
            printf("============ 2.增加联系人=============\n");
            printf("============ 3.删除联系人=============\n");
            printf("============ 4.查找联系人=============\n");
            printf("============ 5.修改联系人=============\n");
            printf("============ 6.销毁通讯录=============\n");
            printf("============ 0.退出通讯录=============\n");
            int choice = 0;
            printf("请输入所需功能：");
            scanf("%d", &choice);
            switch (choice)
            {
                case DISPLAY:
                   AddrBookDisplay(&addr_book);
                break;
                case ADD:
                   AddrBookAdd(&addr_book);
                   //AddrBookSave(&addr_book);
                break;
                case EXIT:
                   flag = 1;
                   printf("退出通讯录！\n");
                break;
               default:
                   printf("请输入正确选择：");
                break;
            }

            if (flag == 1)
            {
                break;//结束退出循环
            }
            
    }
}
////打印通讯录
void AddrBookDisplay(AddrBook* addr_book)
{
       if (addr_book == NULL)
       {
           return;
       }
       //打印对应的数据
       printf("\n====================通讯录=========================\n");
       printf("|   姓名   |   号码   |   地址   |\n");
       printf("|----------------|---------------|----------------|\n");
       size_t i =0;
       for (i = 0; i < addr_book->size; i++)
       {   
           //打印对应数组下面的数据
           printf("|   %s   |   %s   |   %s   |\n", 
           addr_book->data[i].name, addr_book->data[i].phone, addr_book->data[i].address);
           printf("|----------------|---------------|----------------|\n");
       }
       
       
}
//通讯录初始化
void AddrBookInit(AddrBook *addr_book)
{
      if (addr_book == NULL)
      {
          return;
      }
      addr_book->size = 0;
      addr_book->capacity = 1;
      addr_book->data  = (PersonInfo*)malloc(sizeof(PersonInfo)*addr_book->capacity);
}
//内存扩容
void AddrBookRelloc(AddrBook *addr_book)
{
     if (addr_book ==NULL)
     {
         return;
     }
     //用的是堆内存 heap
     addr_book->capacity = addr_book->capacity * 2 +1; //内存不够，扩大容量
     PersonInfo *new_data = (PersonInfo*)malloc(sizeof(PersonInfo)*addr_book->capacity);
     //保留之前就内存的数据
     size_t i =0;
     for (; i < addr_book->size; i++)
     {
         new_data[i] = addr_book->data[i];
     }
     free(addr_book->data);
     addr_book->data = new_data;
}
//添加联系人
void AddrBookAdd(AddrBook * addr_book)
{
     
       if (addr_book == NULL)
       {
            return;// 指针节点为空的处理
       }
       //检查内存容量是否足够大 不够大则扩容
       if (addr_book->size >= addr_book->capacity)
       {
            AddrBookRelloc(addr_book);
       }
       
       //获取书的容量的大小
       size_t cur = addr_book->size;
       ++addr_book->size; //书的容量加一
       /********************添加显示的内容 start*********************************/
        printf("添加联系人\n");
        printf("输入姓名：");
        scanf("%s", &addr_book->data[cur].name);
        printf("输入号码：");
        scanf("%s", &addr_book->data[cur].phone);
        printf("输入地址：");
        scanf("%s", &addr_book->data[cur].address);
        printf("添加结束!\n");
       /********************添加显示的内容 end*********************************/
}
//将文件中的内容加载回内存
void AddrBookLoad(AddrBook *addr_book)
{
    if (addr_book = NULL){
        printf("通讯录为空！\n");
        return;
    }
    
    FILE* fp = fopen(FILE_PATH,"r");
    if (fp = NULL){
        printf("打开文件失败!\n");
        return;
    }
    while (!feof(fp))
    {
        if (addr_book->size >= addr_book->capacity){
            AddrBookRelloc(addr_book);
        }
        size_t cur = addr_book->size;
        fscanf(fp, "%s%s%s\n", addr_book->data[cur].name, addr_book->data[cur].phone, addr_book->data[cur].address);
        addr_book->size++;
    }
    fclose(fp);
}