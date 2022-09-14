#include "AddressBook.h"
//加入数据的信息提示
void MyScanf(message arr[num], int* i)
{
     printf("请输入联系人姓名：\n");
     scanf("%s", (arr[*i].name));
     printf("请输入联系人年龄：\n");
     scanf("%d", &(arr[*i].age));
     printf("请输入联系人电话号码：\n");
     scanf("%s", (arr[*i].number));
     printf("请输入联系人性别：\n");
     scanf("%s", (arr[*i].sex));
     printf("请输入联系人地址：\n");
     scanf("%s", (arr[*i].adress));
}
//添加用户信息
void AddMessage(message arr[num], int* i)
{
    MyScanf(arr,i);
    (*i)++;//偏移量自增
}
// 显示对应的数据
void Show(message arr[num],int i)
{
    int j;
    for(j = 0;j < i;j++)
    {
          MyPrintf(arr, j);
    }
}
//打印数据
void MyPrintf(message arr[num],int j)
{
    printf("%s ", arr[j].name);
    printf("%d ", arr[j].age);
    printf("%s ", arr[j].number);
    printf("%s ", arr[j].sex);
    printf("%s ", arr[j].adress);
    printf("\n");
}
// 查找通讯录的信息
void Find(message arr[num],int i)
{
   char name[10] = {0};
   printf("请输入你要查找的名字:\n");
   scanf("%s",name);
   int k =0; int j;
   for(j =0;j< i;j++)
   {     
         //对比查找的名字
         if(strcmp(name,arr[j].name) == 0)
         {
              k = 1;//更改找到的标识
              MyPrintf(arr,j);
              break; //跳出循环不在往下执行
         }

   }
   if(k==0)
   {
       printf("没有找到你要查找的人");
   }
}
//修改
void Modifies(message arr[num],int* i)
{
     printf("请输入需要修改的名称!");
     char name[10] = {0};
     scanf("%s",name);
     int j,k = 0;
     for(j = 0;j<(*i);j++)
     {       
            //比对对应的名称
            if (strcmp(arr1, arr[j].name) == 0)
            {
                k = 1;
                MyPrintf(arr, j); // 找到对应的数据
                int* a = &j;
                MyScanf(arr, a); //输入对应的数据
                break; //数据不在往下执行
            }
     }
     if( k == 0){
         printf("通讯录中没有此人！！！\n");
     }
}
//删除对应的数据
int Delete(message arr[num], int* i)
{
    
}