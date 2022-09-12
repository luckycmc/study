#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define num 100

enum menu  //定义菜单
{
    ADD = 1,
    SHOW,
    MODIFIES,
    FIND,
    DELETE,
    SORT,
    EXITS,
};

typedef struct s{
    
      int age;
      char name[10];
      char number[20];
      char sex[5];
      char adress[20];
}message;


extern void Choose();

extern void AddMessage(message arr[num],int*);
extern void Show(message arr[num], int);
//extern void Find(message arr[num],int);
extern void MyPrintf(message arr[num], int);
//extern void Modifies(message arr[num],int*);
//extern int Delete(message arr[num], int*);
//extern void SortByname(message arr[num], int);


