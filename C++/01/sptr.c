#include <stdio.h>
// ����ָ��Ҳ��һ��ָ�룬��ָ������ָ��
 // int(*pf) (int, int) ����ָ�� ���͸�ʽ��ͬ�Ϳ��� pf���Ǻ���ָ�� 
void test()
{
	printf("haha\a");
}

int main()
{   
    printf("%p\n",test);
	printf("%p\n",&test); 
	return 0;
}
