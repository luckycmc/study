#include<stdio.h>
 
 int add(int x,int y)
 {
 	return x +y;
 } 
 
 
 int main()
 {
 	int (*p)(int,int); //����һ������ָ��  �������� ��*���������������б� 
	 p = add; // �Ѻ��� add �ĵ�ַ��ֵ�� p ��ֵ��ʱ��һ��Ҫȷ��������ͬ 
	 printf("add(10,20)=%d\n",(*p)(10,20));
    return 0; 
 }
