#include <stdio.h>
//�˵� 
void menu()
{
	  printf("*****************\n");
      printf("***1.sum 2.sub***\n");
      printf("***3.mul 4.div***\n");
      printf("*****0.�˳�*****\n");
      printf("*********\n");
} 
int add(int x, int y)
{
    return x + y;
}
int sub(int x, int y)
{
    return x - y;
}
int mul(int x, int y)
{
    return x * y;
}
int div(int x, int y)
{
    return x / y;
}

//��װһ������ĺ��� ֻ��Ҫ
void calc(int(*pf)(int,int))
{
	int x =0;
	int y =0;
	int ret = 0;
	printf("��������������");
	scanf("%d%d",&x, &y);
	ret = pf(x,y);  //����ָ����ݴ���ĺ�����������Ӧ�ķ��� 
	printf("%d\n",ret); 
} 
int main()
{
	int input = 0;
	do{
		menu();
		printf("��ѡ��:");
		scanf("%d",&input);
		//��switch ��֧�ж�����
		switch(input)
		{
			case 1:
				calc(add); 
				break;
			case 2:
				calc(sub);
				break;
			case 3:
				calc(mul);
				break;
			case 4:
				calc(div);
				break;
			case 0:
				printf("�˳���������\n");
				break;
			default:
			   printf("ѡ�Ŵ���\n");
			   break;	
		} 
	}while(input);
	return 1;
} 
