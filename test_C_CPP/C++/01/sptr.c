#include <stdio.h>
// ����ָ��Ҳ��һ��ָ�룬��ָ������ָ��
 // int(*pf) (int, int) ����ָ�� ���͸�ʽ��ͬ�Ϳ��� pf���Ǻ���ָ��  
 // https://www.jb51.net/article/255237.htm ʵ�ּ������ĵ�ַ 
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
