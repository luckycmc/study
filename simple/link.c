#include <stdio.h>
#include <stdlib.h>

//链表中的结构

typedef struct Link{
	
	  int elem; //数据元素
	  struct Link *next; //指向下一个节点 
}link;

//初始化链表函数

link *initLink();

// 输出打印函数

void display(link *p);

//主函数

int main(){
	
	//链表初始化（1,2,3,4）
	printf("链表初始化\n"); 
	
	link *p = initLink();
	//打印输出链表中的数据
	display(p);
	
	return 0; 
	 
} 

//添加的链表函数

link *initLink(){
	
	link *p = NULL;//创建头指针
	link * temp = (link*)malloc(sizeof(link));//创建首元节点
	
	//首选节点初始化
	temp->elem = 1;
	temp->next = NULL;
	//节点p指向指针的原节点
	p = temp;
	//像各个节点添加数据
	int i;
	for(i =2;i<5;i++){
		   
		    link *a=(link*)malloc(sizeof(link));
            a->elem=i;
            a->next=NULL;
            temp->next=a;
            temp=temp->next;

	} 
	
	return p;
	
} 

//遍历链表

void display(link *p){
	  
	   //讲指针指向头节点
	   link *temp = p;
	   //只要temp 指针指向的不是NULL 就继续向下执行
	   while(temp != NULL){
	   	   
	   	     printf("%d",temp->elem);
		     //指针重置
			 temp = temp->next;		 
	   } 
	   printf("\n"); 
} 






