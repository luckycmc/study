#include <stdio.h>

int main()
{   
    //定义结构体 
	struct{
		
		char *name;
		int num;
		int age;
		char group;
		float score;
		
	} stu1,stu2 = {"Tom", 12, 18, 'A', 136.5};
    
	//给结构体赋值
	stu1.name = "小张";
	stu1.num  = 12;
	stu1.age  = 26;
	stu1.group = 'A';
	stu1.score = 99.5;
	//读取结构体的值 
	printf("%s的学号是%d，年龄是%d，在%c组，今年的成绩是%.1f！\n", stu1.name, stu1.num, stu1.age, stu1.group, stu1.score);
	printf("\n"); 
	printf("%s的学号是%d，年龄是%d，在%c组，今年的成绩是%.1f！\n", stu2.name, stu2.num, stu2.age, stu2.group, stu2.score);
} 
