#include <stdlib.h>
#include "stdio.h"

struct Date{
    int year;
    int month;
    int day;
};

struct Book{
//    char *title;
    char title[30];
    struct Date date;
};

void  getInput(struct Book *book){
    printf("请输入书名: ");
    scanf("%s",book->title);
    printf("请输入出版日期: ");
    scanf("%d-%d-%d",&book->date.year,&book->date.month,&book->date.day);
}

int main( void ){
    struct Book *b1;
    b1 = (struct Book *)malloc(sizeof (struct Book));

    if(b1 == NULL){
        printf("内存分配失败:\n");
        exit(1);
    }

    getInput(b1);
    printf("书名:%s\n",b1->title);

//    printf("出版日期:%d-%d-%d\n",b1.date.year,b1.date.month,b1.date.day);

    return 0;
}