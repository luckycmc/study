#include <stdio.h>

//创建结构体

struct Student{
	 
	  char *name;
	  int age;
}; 

struct Student stu1 ={
	"MJ",
	27
};

int main(){
	printf("age is %d",stu1.age);
}
