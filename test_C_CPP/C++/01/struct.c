#include <stdio.h>
#include <string.h>
//����ṹ��
struct  Student{
	int age;
	float score;
	char name[100];
}; 

//��������

void intput_student(struct Student *p);  //��� const ������ ���ܱ��ı� 
void output_student(struct Student *p); 

int main()
{   
     struct Student p;
     intput_student(&p);  //�ṹ���������ݻ���Ÿı� 
     output_student(&p); 
	 return 1;
} 
//����ѧ�������� 
void intput_student(struct Student *p){
	 p->age = 30;
	 p->score = 90.5;
	 strcpy(p->name,"��ɺ");
}
 
void output_student(struct Student *p){
	printf("%d %f %s\n", (*p).age, p->score, p->name);
}

