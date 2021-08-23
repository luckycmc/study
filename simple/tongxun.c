#include <stdio.h>
#include<string.h>
#include<stdlib.h>
#include<windows.h>
typedef struct student
{
   	  char name[20];    //姓名
      char num[15];      //学号
    
      struct student *next;  //结构体指针 指向下一个节点的存放数据点 
}stu;
//结构体头指针 
stu *head;
//屏幕显示 
void screen();
//主菜单
void print(); 
//用户添加数据
void input(stu *p1);
//数据输入的函数
stu * inputdata();
//显示数据 
stu * lookdata(stu *p1);
//插入数据
void insert();    
//删除对应的数据 
void deleted();    
//查找对应的数据
void find(stu *p2);  
 //通过姓名查找修改数据    
void update(stu *p2); 

//保存数据信息
void save(stu *p2); 

//主函数 
int main()
{
	int i;
    system("color 4e");
	screen();
	//延时30秒 
	Sleep(3000);
	//显示主菜单 
	print();
	//获取用户输入的信息
	while(1)
	{
		printf("请输入你的选择(1~9):");
        loop:scanf("%d",&i);
         if(i<1||i>9)
         {
             printf("输入有误,请在1~9中进行选择:");
             goto loop;
         }
         //查找对应的功能 
         switch(i)
		 {
		 	case 1:
		 		inputdata();
		 		break;
		 	case 2:
		 		lookdata(head);
		 		break;
		 	case 3:
			   insert();
			   break;	
			case 4:
			   deleted();
			   break;
			case 5:
			   find(head);
			   break;
			case 6:
			   update(head);
			   break;
			case 7:
			   save(head);
			   break;
			case 8:
			   print();
			   break;
		    case 9:
		    	printf("退出登录\n");
			    exit(1);
				break;		  
		 }
	} 
}
//保存数据 
void save(stu *p2)
{
	FILE *fp;
	char file[15]; //文件名称
	printf("\n\t\t\t☆☆☆保存数据☆☆☆\n"); 
	printf("----------------------------------------------------------------------\n");
	printf("输入文件名:");
	scanf("%s",&file);
	//以写的方式打开文件 
	if((fp=fopen(file,"w"))==NULL)
	{
		printf("cannot open this file\n");
		exit(0);
	} 
	fprintf(fp,"姓名\t学号\n");
	//读取数据文件
	while(p2 != NULL)
	{
		 fprintf(fp,"%s\t",p2->name);
         fprintf(fp,"%s\t",p2->num);
         //指针后移
		 p2 = p2->next; 
	} 
	
	printf("\n保存成功!\n");
	printf("======================================================================\n");
	fclose(fp); //关闭文件 
}
//更新对应的数据 
void update(stu *p2)
{
	
	 char name [20];
	 int b=0,i;
	 printf("\n\t\t\t☆☆☆修改数据☆☆☆\n");
	 printf("----------------------------------------------------------------------\n");
	 printf("请输入将要修改人的姓名:");
	 scanf("%s",name);
	 //1.有可能是空链表空数据要处理下
	 if(p2 == NULL)
	 {
	 	
	 	 printf("通讯录数据为空哦\n");
	 	 return ;
	 } 
	 //链表中有数据的时候
	 while(p2 != NULL)
	 { 
	       
		   //查找输入的名称
		   if(strcmp(name,p2->name) == 0)
		   {      
		         /****************输入当前学生的基本信息 start**********************/
		   	     printf("该同学的基本信息\n");
		   	     printf("姓名:%s\n",p2->name);
		   	     printf("学号:%s\t",p2->num);
		   	     /****************输入当前学生的基本信息 end**********************/
		   	     printf("\n请选择要修改的信息\n");
		   	     printf("\t1.姓名\t2.学号\n");
		   	     printf("\n您的选择是(1~8):");
		   	     scanf("%d",&i);
		   	     printf("请输入修改之后的内容\n");
		   	     //更具标识修改对应的内容 
		   	     switch(i)
				 {
				 	 case 1:
					   printf("请输入姓名:");
					   scanf("%s",&p2->name);
					   break;
					 case 2:
					    printf("请输入学号:");
						scanf("%s",&p2->num);
						break;
					  
				 }
				 //提示修改成功！
				 printf("\n修改成功!\n");
				 printf("----------------------------------------------------------------------\n");
				 b = 1; //更新修改标识 
		   }	
		   
		   //指针节点往后移动
		   p2 = p2->next;
		   
		  
	 } 
	  //判断该学生的信息是否存在
		if(b == 0)
		{  
		   	  printf("没有找到该人的资料\n");
		} 
		return ;
}
//查找对应的函数 
void find(stu *p2)
{
	 char name[20];
	 int b = 0;
	 printf("\n\t\t\t☆☆☆查看数据☆☆☆\n");
	 printf("----------------------------------------------------------------------\n");
	 printf("请输入您想查找人的姓名:");
	 //获取输入人的姓名
	 scanf("%s",name);
	 //1.有可能是空链表空数据要处理下
	 if(p2 == NULL){
	 	
	 	 printf("通讯录数据为空哦\n");
	 	 return ;
	 } 
	 //遍历链表查找对应的数据
	 while(p2!=NULL)
	 {     
	       //找到对应的人员做对应的提示操作 
	 	  if(strcmp(name,p2->name) == 0)
		   {
		   	   printf("你要找到的数据\n");
               printf("姓名:%s\n",p2->name);
               printf("学号:%s\t",p2->num);
               printf("\n");
               //标识找到对应的数据了 
               b = 1; 
		   }
		   //指针节点往后移动
		   p2 = p2->next; 
	 } 
	 //判断数据是否存在 
	 if(b == 0)
	 { 
	     printf("\n您要查找的人不存在!\n");
		 return ;	
	 }
}
//删除数据 
void deleted() 
{
    stu *p1,*p2; //声明两个结构体指针
    char Name[20]; //需要删除用户的名称
	printf("\n\t\t\t☆☆☆删除数据☆☆☆\n");
    printf("----------------------------------------------------------------------\n");
    printf("请输入要删除的姓名:");   
    //获取用户输入的名称
	scanf("%s",Name);
	p1 = head ; //p1 指针指向头结点
	// 节点中没有数据
	if(head == NULL)
	{
		printf("内存空空神马都没有!\n");
		printf("======================================================================\n");
		return ;
	} 
	//节点中只有一个数据
	if(strcmp(Name,p1->name)==0)
	{
		head = p1->next;
		printf("数据删除成功!");
		printf("======================================================================\n");
		return ;
	} 
	
	//查找对应的数据
	while(p1!=NULL && (strcmp(Name,p1->name)!=0))
	{
		p2 = p1;
		p1 = p1->next; //指针节点后移 
	} 
	//删除的人员可能不存在
	if(p1 == NULL)
	{
		printf("删除的人员不存在！");
		printf("======================================================================\n");
		return; 
	} 
	//相关人员存在则删除
	if(p1->next!=NULL){
		 
		 p1 = p1->next;//p1 移动到下一个指针节点
		 //辅助指针指向p1
		 p2->next = p1;
		 printf("删除成功!\n"); 
		 printf("======================================================================\n");
		return;  
	} else{
	     
	     p2->next = NULL;
	     printf("删除成功\n");
	     printf("======================================================================\n");
	     return;
	} 
}
 //插入数据
void insert()     
{
    int i;
    char named[20];//输入的名称
	stu *p1,*p2,*p3;
	p1 = head;
	p3=(stu *)malloc(sizeof(stu));
	p3->next=NULL;
    printf("\n\t\t\t☆☆☆插入数据☆☆☆\n");
    printf("----------------------------------------------------------------------\n");
    printf("请输入插入者的资料:\n");
    //记录p3 指针的值 
    input(p3);
    printf("\n插入选项\n");
    printf("1.首位置插入\t2.尾位置插入\t3.前插\n");
    printf("请输入你的选择:");
	scanf("%d",&i); 
	//插入头部尾部或者前插
	switch(i)
	{
		case 1:
			p3->next=p1;
			head=p3;
			break;
		case 2:
			    while(p1->next!=NULL)
				{
					p2 = p1;
					p1 = p1->next;
				}	
			p1->next = p3;
			break;
		case 3:
		     printf("请输入姓名（前插）:");
			 scanf("%s",named);
			 //查找对应的名称节点 
			 while(strcmp(named,p1->name)!=0)
			 {
			 	 p2 = p1;
			 	 p1 = p1->next;
			 }	
			 p2->next=p3;
			 p3->next=p1;
			 break;
	} 
	printf("插入成功!\n");
	printf("======================================================================\n");
	return;
}
//查看数据的函数
stu * lookdata(stu *p1)     
{
	 printf("\n\t\t\t☆☆☆显示数据☆☆☆\n");
	 printf("----------------------------------------------------------------------\n");
	 if(p1 == NULL){
	 	 
	 	 printf("--------------------------暂无数据哦~----------------------------------\n");
	 	 return NULL; 
	 } 
	 //显示对应的数据 
	 while(p1!=NULL)
	 {
	 	 printf("姓名:%s\n",p1->name);
         printf("学号:%s\t",p1->num);
         printf("======================================================================\n");
         p1=p1->next;
	 } 
	 return p1;
} 
////数据输入的函数
stu * inputdata()     //数据输入的函数
{
	stu *p1,*p2;
	int i =1;
	p1 = (stu *)malloc(sizeof(stu));
	//内存申请失败直接退出 
	if(p1 != NULL)
	{
		 head = p1;
         printf("\n\t\t\t☆☆☆输入数据☆☆☆\n");
         printf("------------------------------------------------------------------\n");
         printf("在姓名处输入“ok”代表输入数据结束\n");
         //读取输入的数据 
         while(i)
		 {    
		       printf("姓名:");
		 	   //判断是否是需要结束
			   scanf("%s",&p1->name);
			   //判断是否需要结束
			   if(strcmp(p1->name,"ok") == 0)
			   {
			   	  printf("\n输入完毕!\n");
                  printf("========================================================================\n");
                  i=0; //退出当前循环 
                  p2->next=NULL; //指向为空 
                  free(p1); //释放内存 
                  p1=p2;    // 
			   }
			   else
			   {
			      printf("学号:");
                  scanf("%s",&p1->num);
                  printf("=====================================\n");
                  p2=p1;
                  //为p1重新申请内存 
                  p1=(stu *)malloc(sizeof(stu));
                  if(p1!=NULL)
                  {
                     p2->next=p1;
                  }
			   } 
		 }
         return(p1->next);
	}
	
} 
//输入相关数据 
void input(stu *p1)
{
	printf("姓名:");
	scanf("%s",&p1->name);
	printf("学号:");
	scanf("%s",&p1->num);
}

//主菜单
void print()    
{
     system("cls");
     printf("\n\n\n");
     printf("      ☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆\n");
     printf("      ***********************************************************\n");
     printf("      \t\t\t班级通讯录管理系统\n");
     printf("\n");
     printf("      \t\t1. 输入数据");
     printf("\t\t2. 显示数据\n");
     printf("      \t\t3. 插入数据");
     printf("\t\t4. 删除数据\n");
     printf("      \t\t5. 查看数据");
     printf("\t\t6. 修改数据\n");
     printf("      \t\t7. 保存数据");
     printf("\t\t8. 返回主菜单\n");
     printf("      ***********************************************************\n");
     printf("      ~~~~~退~~~~~~~~~~出~~~~~~~~~~请~~~~~~~~~~按~~~~~~~~~~9~~~~~\n");
    printf("      -----------------------------------------------------------\n\n");
}
//屏幕显示的主函数
void screen()
{
	int i;
	//定义现实的字符串 
	char s[251] = {"欢迎使用由ZM制作班级通讯录管理系统，\n\n\t\t\t本系统用于通讯录管理----排序，打印\n\n\n\t\tWelcome to use produced by ZM class address book\n\n\t\t management system,sorting,printing"};
	printf("\n================================================================================\n");
	printf("\n\n\n\t\t\t");
	//打开现实的数据 
	for(i=0;s[i]!=NULL;i++)
	{
		Sleep(30); 
		printf("%c",s[i]);
	} 
	printf("\n\n\n\n\n\n\n\n\t\t ~ Hi~ o(*￣▽￣*)ブ~ ~ ~祝您旅途愉快~ ~\n");
    printf("================================================================================\n");
} 

