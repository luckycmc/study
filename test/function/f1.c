#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Person Person;

typedef void (*Method)(Person *my_self);

typedef struct Person{
    char name[12];
    int age;
    int sex;
    Method behavior1; //行为
}Person;
//自我介绍函数
void selfIntroducation(Person *my_self) 
{
    printf("my name is %s,age %d,sex %d\n",my_self->name,my_self->age,my_self->sex);
}
//主函数
int main()
{     
      //创建结构体指针
      Person *pmst = (Person *)malloc(sizeof(Person));
      //给对象赋值
      strcpy(pmst->name,"pmst");
      pmst->age = 18;
      pmst->sex = 0;

      // 2
      pmst->behavior1 = selfIntroducation;  //指针指向对应的函数
     // 3
      pmst->behavior1(pmst);   //调用对应的函数

     return 0;
}