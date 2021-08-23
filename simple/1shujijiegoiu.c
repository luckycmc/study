#include <stdio.h>
#include <stdlib.h>

//定义相应的数据表结构
typedef struct Table{
	
	 int *head;// //声明了一个名为head的长度不确定的数组，也叫“动态数组”
	 int length; //记录当前顺序表的长度
	 int size; // 记录顺序表的存储容量 
} table; 

#define Size 5

//gcc c89 for 只能先声明变量然后在执行 
table initTable(){
	table t;
	t.head = (int*)malloc(Size*sizeof(int)); // /构造一个空的顺序表，动态申请存储空间
	//如果申请失败做出正常提示 然后退出
	
	if(!t.head){
		
		printf("初始化失败");
		exit(0); 
	} 
	
	t.length =0; //空表的长度初始化为0
	t.size  = Size;  //空表的初始存储空间为Size
	return t;
}

//输出顺序表中元素的函数
void displayTable(table t){
	int i;
    for (i=0;i<t.length;i++) {
        printf("%d ",t.head[i]);
    }
    printf("\n");
}

//入口函数
int main(){
   
   table t = initTable();
   //向顺序表中 添加数据
   int i;
   for(i=1;i<=Size;i++){
   	   
   	    t.head[i-1]=i;
   	    t.length++;
   } 
    printf("顺序表中存储的元素分别是：\n");
    displayTable(t);
    return 0;
} 







