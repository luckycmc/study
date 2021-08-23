#include <stdio.h>
#include <stdlib.h>

//定义链表结构体
typedef struct Link 
{
	
	 int elem;            //链表的数据域 
	 struct Link *next;   //链表的指针域 
} link;
//初始化链表
link *initLink();

//链表展示数据
void dispay();

// 查找链表中的元素
void find(link *p);

//修改某个节点的数据
link *edit(link *p,int old,int new);

//修改某个节点的数据
link *del(link *p,int elem);

//主函数 
int main()
{
	
	//链表初始化
	printf("数据链表初始化\n");
	//链表初始化 
	link *p = initLink();
	display(p);
    //查找对应的数据
	//find(p);
	link *p1 = edit(p,10,50);
	// 打印展现链表数据 
    display(p1);
    
    //link *p2 = del(p,3);
	// 打印展现链表数据 
    //display(p2);
	return 0;
}
//删除当前元素的节点 
link *del(link *p,int elem)
{
	int flag = 0;
	//将temp 指针重新指向头结点
	  link *temp = p;
	  //只有temp  不等于 NULL的时候节点数据继续往下走 
	  while(temp->next != NULL)
	  {
	  	    temp = temp->next;   //指向下一个节点 
	  	    if(temp->elem == elem)
		    {
		   	   printf("找到了该数据删除成功！");
		   	   temp = temp->next->next; 
		   	   flag =1;
			   break; 
		    }
	  } 
	  if(flag == 0)
	  {
	  	  printf("没有找到需要删除的数据哦!");
	  	  return NULL;
	  }
	  return p;
} 
//修改当前元素的节点 
link *edit(link *p,int old,int new)
{    
      int flag = 0;
	//将temp 指针重新指向头结点
	  link *temp = p;
	  //只有temp  不等于 NULL的时候节点数据继续往下走 
	  while(temp->next != NULL)
	  {
	  	    temp = temp->next;   //指向下一个节点 
	  	    if(temp->elem == old)
		    {
		   	   printf("找到了该数据,替换成功！");
		   	   temp->elem = new; 
		   	   flag =1;
			   break; 
		    }
	  } 
	  if(flag == 0)
	  {
	  	  printf("没有找到需要替换的数据哦!");
	  	  return NULL;
	  }
	  return p;
}
//查找对应的元素 
void find(link *p)
{     
      int elem;
      scanf("%d",&elem);
      int flag = 0;
	 //将temp 指针重新指向头结点
	  link *temp = p;
	  while(temp->next != NULL)
	  {
	  	   temp = temp->next;   //指向下一个节点 
	  	   if(temp->elem == elem)
		   {
		   	   printf("找到了该数据");
		   	   printf("%d",temp->elem);
		   	   flag =1;
			   break; 
		   }
		  
	  }
	  //提示没找到对应的数据 
	  if(flag == 0)
	  {
	  	   printf("没找到了该数据");
	  }
	 	  
}
//数据展示 
void display(link *p)
{
	 
	  //将temp 指针重新指向头结点
	  link *temp = p;
	  //只有temp  不等于 NULL的时候节点数据继续往下走 
	  while(temp->next != NULL)
	  {
	  	    temp = temp->next;   //指向下一个节点 
	  	    printf("----"); 
	  	    printf("%d",temp->elem); 
	  	    printf("\n");
	  } 
	   
} 
//链表数据初始化
link *initLink()
{
	
	 link *p = (link*)malloc(sizeof(link)); //创建一个头结点 声明一个指针空间 
	 link *temp = p; //声明一个指针指向向节点,用于遍历链表
	 //生成链表
	 int i;
	 for(i=0; i<5; i++){
	 	  
	 	   link *a = (link*)malloc(sizeof(link));
	 	   a->elem = i;
	 	   a->next = NULL;
	 	   temp->next = a;
	 	   //temp = temp->next;
	 	   temp = a;//修改头结点的指向 
	 }
	 return p; 
} 

