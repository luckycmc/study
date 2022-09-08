#include <stdio.h>
#include <string.h>
//定义结构体
struct  Student{
	int age;
	float score;
	char name[100];
}; 

//声明函数

void intput_student(struct Student *p);  //如果 const 修饰则 不能被改变 
void output_student(struct Student *p); 

int main()
{   
     struct Student p;
     intput_student(&p);  //结构体引用数据会跟着改变 
     output_student(&p); 
	 return 1;
} 
//输入学生的数据 
void intput_student(struct Student *p){
	 p->age = 30;
	 p->score = 90.5;
	 strcpy(p->name,"张珊");
}
 
void output_student(struct Student *p){
	printf("%d %f %s\n", (*p).age, p->score, p->name);
}

