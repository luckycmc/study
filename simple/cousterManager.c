#include <stdio.h>
#include <string.h>

//定义客户结构体 
struct Customer {
	int   id;
	int   age;
	char  name[10];
	char  gender;
	char  phone[10];
	char  email[10];
};

int key ;
char loop=1;
int  customerNum=1;
//添加客户 
void add()
{
	printf("添加客户信息\n"); 
}
//添加客户 
void del()
{
	printf("删除客户信息\n"); 
}
//添加客户 
void showList()
{
	printf("显示客户信息\n"); 
}
//添加客户 
void edit()
{
	printf("修改客户信息\n"); 
}
//定义显示菜单
void mainMenu()
{
	do{
		
		printf("\n----------------------客户关系管理系统--------------------------");
		printf("\n                       1 添 加 客 户                            ");
		printf("\n                       2 修 改 客 户                            ");
		printf("\n                       3 删 除 客 户                            ");
		printf("\n                       4 客 户 列 表                            ");
		printf("\n                       5 退       出                            ");
		printf("\n                       请选择(1-5)：_                           ");
		
		//获取对应的输入数据
		printf("\n请输入(1-5):");
		scanf("%d",&key); 
		//选择对应的操作功能
		switch(key)
		{
			case 1:
				add(); 
				break;
			case 2:
				edit();
				break;
			case 3:
				del();
				break;
			case 4:
				showList();
				break;
			case 5:
				loop = 0;
				break;
		    default:
		    	printf("你选择的选择不对哦");
				break;				
		} 
	}while(loop);
} 
//主函数 
void main()
{
	mainMenu();
}
