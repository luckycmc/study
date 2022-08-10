#include <stdio.h>
#include <stdlib.h>
#define OK 1
#define ERROR 0
#define TRUE 1 
#define FALSE 0 
#define SUCCESS 1 
#define UNSUCCESS 0 
#define HASHSIZE 7          
#define NULLKEY -32768

typedef int Status;

//hash 结构体
typedef struct {
	int *elem;   //基址
	int count;   //当前数据元素个数
}HashTable; 

int m =0;//散列表的长度
//hashTable 初始化 
int init(HashTable *hashTable)
{
	 int i; m = HASHSIZE;
	 hashTable->elem = (int *)malloc(m * sizeof(int));
	 if(hashTable->elem == NULL){
	 	
	 	  printf("application memy is error\n");
	 	  return 0;
	 }
	 hashTable->count = m;
	 for(i =0;i<m;i++)
	 {
	 	 hashTable->elem[i] = NULLKEY;
	 }
	 return 1;
}
//hash 函数 求余 
int hash(int data)
{
	return data%m;
} 
//数据插入hashTable
void insert(HashTable *hashTable,int data)
{
	  int hashAddress = hash(data); //返回的地址不在限定范围内 
	  //地址发生冲突 ==NULLKEY 说明还没有被占用 否则呗使用了  {13,29,27,28,26,30,38}
	  while(hashTable->elem[hashAddress] != NULLKEY) //当前数组已经有数据存在了 
	  {
	  	   //利用开放性探测寻址发 
	  	   hashAddress = (++hashAddress)%m; //直到找到一个合适的为止 
	  	   printf("data is %d---",data);
	  	   printf("hashAdress is %d\n",hashAddress);
	  } 
	 
	  //插入值
	  hashTable->elem[hashAddress] = data;
} 
//数据查找
int search(HashTable *hashTable,int data)
{
	  //先求hash地址
	  int hashAdress = hash(data);
	  
	  //解决发生冲突的数据
	  while(hashTable->elem[hashAdress] != data)
	  {
	  	   //利用开放定址的线性探测法解决冲突 
		    hashAdress=(++hashAdress)%m; 
		  
		    if (hashTable->elem[hashAdress]==NULLKEY||hashAdress==hash(data)) return -1; 
	  } 
	  return 1;//数据查找成功 
} 
//数据展现 
void display(HashTable *hashTable)
{
	  int i; 
	  printf("\n//==============================//\n"); 
	  
	  for (i=0;i<hashTable->count;i++) 
	  { 
	     printf("%d",hashTable->elem[i]); 
	  } 
	  
	  printf("\n//==============================//\n"); 
}
//主函数 
int main()
{    
      int i,j,result; 
	  HashTable hashTable; 
	  int arr[HASHSIZE]={13,29,27,28,26,30,38}; 
	  
	  printf("***************Hash哈希算法***************\n"); 
	  
	  //初始化哈希表 
	  init(&hashTable); 
	  
	  //插入数据 
	  for (i=0;i<HASHSIZE;i++) 
	  { 
	     insert(&hashTable,arr[i]); 
	  } 
	  //打印对应的数据 
	  display(&hashTable); 
	  //查找对应的数据
	  result = search(&hashTable,34);	 
	  if(result == -1){
	  	    printf("对不起，没有找到！\n");
	  	    return -1;
	  }
	  printf("29在哈希表中的位置是:%d\n",result);
	  return 1;
} 

// https://www.cnblogs.com/shuly/p/12198455.html 
