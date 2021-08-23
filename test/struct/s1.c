#include <stdio.h>
#include <stdlib.h>

#define NAMESIZE 32
//结构体的成员引用
/**
  变量名.成员名
  指针->成员名
  (*指针).成员名
*/
struct simple_st
{
   int i;
   float f;
   char ch;
};
//  结构体内存对齐 simple_st 暂用 12个字节
struct birthday_st
{
     int year;
     int month;
     int day;
}
//类型 变量名
struct student_st
{
   int i;
   char name[NAMESIZE];
   struct birthday_st birth; //嵌套的结构体
   int math;
   int chinaese;

}
//结构体传参数
void func(struct simple_st b)
{

}
//主函数
int main()
{  

    // TYPE  NAME =VALUE;
    struct student_st stu = {10011,"Alan",{2011,11,11},98,97};//给结构体成员赋值
    //结构体指针
    struct student_st *p = &stu; //定义结构体指针
    struct student_st arr[2]={{},{}}//定义结构体数组 
    //打印查看对应的值
    printf("%d %s %d-%d-%d %d %d\n",stu.id,stu.name,stu.birth.year,stu.birth.month,stu.birth.day,stu.math,stu.chinaese);

    // func(a)  结构传参 内存开销比较大 一般是指针传过去    
}