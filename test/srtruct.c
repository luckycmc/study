#include <stdio.h>

struct student{
     int num;
     char a;
     struct student *next;
     double sum;
     
};

int main()
{
    printf("占用的字节数:*******%d\n",sizeof(struct student));
}