#include<stdio.h>
/**
  ʵ�� 9*9 �˷��� 
**/
int main()
{
	//������Ҫ�������� �ͽ��
	int i,j,result;
	printf("\n"); //�������
	//��һ����� 9������ 
	for(i=1;i<10;i++)
	{
		for(j=1;j<=i;j++) //�����һ�� 1*1 1*2 *2.... 
		{
			 //�����Ӧ�Ľ��
			 result = i * j;
			 printf("%d*%d=%-3d",j,i,result); /*-3d��ʾ����룬ռ3λ*/ 
		} 
		//ÿһ�л�һ����
		printf("\n"); 
	} 
	return 0;
} 
