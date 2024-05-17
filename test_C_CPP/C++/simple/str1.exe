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
//添加顺序数据
table addTable(table t,int elem,int add){
      
	    //判断位置是否正确
		if(add > t.length+1 || add <1){
			
			 printf("插入位置有问题\n");
			 
			 return t;
		}
		
		//重新分配内存
		if(t.length > t.size){
			 
			  t.head=(int *)realloc(t.head, (t.size+1)*sizeof(int));
			  if(!t.head){
			  	  
			  	   printf("存取分配失败\n");
			  }
			  t.size+=1;
		}	
		
		//插入对应的数据  指定的位置留给对应的值 其他的全部后移
		int i;
		for (i=t.length-1;i>add-1;i--){
			  t.head[i+1] = t.head[i];
		}
		//复制当前的数据位置
		t.head[add] =  elem;
		t.length++; // 长度加 1 
		 
} 
//删除对应的数据
table delTable(table t,int add){
	  
	   //判断删除的数据是否存在
	   if(t.length<add || add < 1){
	   	    
	   	    printf("删除的数据位置有错误!");
	   	    return t;
	   } 
	   int i;
	   for(i=add;i<t.length;i++){
	   	   t.head[i-1] = t.head[i];
	   }
	   t.length--;
	   return t;
} 
//查找对应的元素
int selectTable(table t,int elem){
	
	  int i;
	  for(i=0;i<t.length;i++){
	  	   
	  	     	 
	  	  if(t.head[i] == elem){
	  	    	      
			  return i+1;  	
			}
			
	  }
	  return -1;
} 
//修改对应的元素
table amendTable(table t,int elem,int newElem){
	  
	   //获取对应元素的下标
	   int add =  selectTable(t,elem);
	   if(add == -1){
	   	    
	   	    printf("查找有误!\n");
	   }
	   t.head[add-1]=newElem;
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
    
    printf("删除元素1:\n");
    t =delTable(t, 1);
    displayTable(t);
    
    printf("在第2的位置插入元素5:\n");
    t = addTable(t, 5, 2);
    displayTable(t);
    
    printf("查找元素3的位置:\n");
    int add=selectTable(t, 3);
    printf("%d\n",add);
    
    //修改
	printf("将元素3改为6:\n");
    t=amendTable(t, 3, 6);
    displayTable(t); 
    return 0;
} 







