#include <stdio.h>
#include <stdlib.h>

//定义一个人数结构体
typedef struct node
{
    int number;
    struct node *next;
}person;

//链表初始化 
person *initLink(int n);
// 查找出局的人数
void findAndKillK(person *head,int k,int m);
//主函数执行调用
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
//环形链表初始化
person *initLink(int n)
{
     
       person * head = (person *)malloc(sizeof(person));
       /*********链表节点的初始化 start********/
       head->number = 1;
       head->next   = NULL;
       /*********链表节点的初始化 end********/
       //使用辅助指针
       person *cycle = head;
       int i;
       for (i = 2; i <=n; i++)
       {
           person *body = (person *)malloc(sizeof(person));
           body->number = i;
           body->next   = NULL;

           /*********和辅助节点产生关联 start*********/
           cycle->next = body;
           cycle       = cycle->next; //移动指针指向的地址  此时指向body也是可以的
           /*********和辅助节点产生关联 end*********/
       }
       // cycle->next->指向头节点
       cycle->next = head; //收尾相连
       return head;
}

//查找对应的数据
void findAndKillK(person *head,int k,int m)
{
     person *tail =head;  

     //查找首节点的上哪一个节点 也就是节点的最后一个数据
     while(tail->next !=head){
            //printf("*****tail->next is********%p\n",tail->next);
            //printf("*****head->next is********%p\n",head);
            tail = tail->next;
     }
     printf("*****tail data is********%d\n",tail->number);
     person * p=head;
    //找到编号为k的人
    while (p->number!=k) {
        tail=p;
        p=p->next;
    }
    //从编号为k的人开始，只有符合p->next==p时，说明链表中除了p结点，所有编号都出列了，
    while (p->next!=p) {
        //找到从p报数1开始，报m的人，并且还要知道数m-1de人的位置tail，方便做删除操作。
        int i;
        for ( i=1; i<m; i++) {
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