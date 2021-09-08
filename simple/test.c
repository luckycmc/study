#include <stdio.h>

#define N 3 //宏定义 

struct student 
{
	int num;
	char name[20];// 学生姓名
	float score[3];//学生成绩
	float aver;   //平均成绩 
}; 
//主函数 
int main()
{
	 //获取学生成绩
	 void input(struct student s[]); 
	 //结构体函数请最大值
	 struct student max(struct student s[]);//函数声明 
	 // 打印函数
	 void print(struct student s); 
	 //定义结构体变量 
	 struct student s[N],*p=s;
	 //调用函数 
     input (p);
     //打印调用max函数结果 
     print(max(p));
     //主函数的返回值 
     return 0;
} 


