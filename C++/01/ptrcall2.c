#include<stdio.h>

typedef struct TFT_DISPLAY
{
	int x;
	int y;
	void (*p)(void); //��ӡ����sss 
	int (*m)(int,int);
}tft_display;

//��ӡ����

void display(void)
{
	printf("display function\n");
} 

// �ӷ�����

int sum(int x,int y){
	return x+y;
} 

tft_display stu = { //�ṹ�帳ֵ���� 
	.x = 1,
	.y = 2,
	.p = display,  //ָ�뺯����ֵ ֻҪ�������� ����ֵ��ͬ �Ϳ��Խ���ָ�� 
	.m = sum,
};
//���ݳ�ʼ�� 
int main()
{
	stu.p();
	printf("sub is %d\n",stu.m(stu.x,stu.y));
	return 1;
}
