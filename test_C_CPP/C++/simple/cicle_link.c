#include<stdio.h>
#include <stdlib.h>

typedef struct node 
{   

	int number;
	struct node *next; //node 的结构体指针 指向下一个节点的地址 
	
}person;

//链表初始化
person *initLink(int n)
{    
    int i; 
	person *head = (person*)malloc(sizeof(person)); //头结点并声明指向空间
	head->number = 1;
	head->next   = NULL;
	//声明辅助节点
	person *cycle = head;
	//创建循环链表 
    for(i=2;i<=n;i++)
	{
	 	person *body = (person*)malloc(sizeof(person));
	 	body->number = i;
	 	body->next   = NULL;  //指针节点指向空 
	 	cycle->next  = body; //指向新节点
		cycle        = body;  //辅助节点后移 
	} 
	//尾部节点指向头结点
	cycle->next = head;
	return head; 
} 
//查找对应的人 
void findAndKillK(person * head,int k,int m)
{
	person *tail = head;
	
	//找到链表的第一个节点 为数据删除做准备
	while(tail->next != head){
		 
		 printf("%d\n",tail->number); 
		 tail = tail->next;  //元素的第一个节点 
		
	} 
	//printf("元素的第一个值是多少:"); 
	//打印查看元素 
	//printf("%d",tail->number); 
	person *p =head;
	//找到编号为k的人
	while(p->number != k)
	{
		tail = p;
		p = p->next;
	} 
	//从编号为k的人开始，只有符合p->next==p时，说明链表中除了p结点，所有编号都出列了
	while (p->next!=p) 
	{   
	     int i;
		//找到从p报数1开始，报m的人，并且还要知道数m-1de人的位置tail，方便做删除操作。
        for (i=1; i<m; i++) {
            tail=p;
            p=p->next;
        }
        
        tail->next=p->next;//从链表上将p结点摘下来
        printf("出列人的编号为:%d\n",p->number);
        
        free(p);
        p=tail->next;//继续使用p指针指向出列编号的下一个编号，游戏继续
	}
	printf("出列人的编号为:%d\n",p->number);
    free(p);
}
//主函数 
int main()
{   
    printf("输入圆桌上的人数n:");
    int n;
    scanf("%d",&n);
    person * head=initLink(n);
    printf("从第k人开始报数(k>1且k<%d)：",n);
    int k;
    scanf("%d",&k);
    printf("数到m的人出列：");
    int m;
    scanf("%d",&m);
    findAndKillK(head, k, m);
    
    return 0;
	
}
