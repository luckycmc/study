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

//hash �ṹ��
typedef struct {
	int *elem;   //��ַ
	int count;   //��ǰ����Ԫ�ظ���
}HashTable; 

int m =0;//ɢ�б�ĳ���
//hashTable ��ʼ�� 
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
//hash ���� ���� 
int hash(int data)
{
	return data%m;
} 
//���ݲ���hashTable
void insert(HashTable *hashTable,int data)
{
	  int hashAddress = hash(data); //���صĵ�ַ�����޶���Χ�� 
	  //��ַ������ͻ ==NULLKEY ˵����û�б�ռ�� ������ʹ����  {13,29,27,28,26,30,38}
	  while(hashTable->elem[hashAddress] != NULLKEY) //��ǰ�����Ѿ������ݴ����� 
	  {
	  	   //���ÿ�����̽��Ѱַ�� 
	  	   hashAddress = (++hashAddress)%m; //ֱ���ҵ�һ�����ʵ�Ϊֹ 
	  	   printf("data is %d---",data);
	  	   printf("hashAdress is %d\n",hashAddress);
	  } 
	 
	  //����ֵ
	  hashTable->elem[hashAddress] = data;
} 
//���ݲ���
int search(HashTable *hashTable,int data)
{
	  //����hash��ַ
	  int hashAdress = hash(data);
	  
	  //���������ͻ������
	  while(hashTable->elem[hashAdress] != data)
	  {
	  	   //���ÿ��Ŷ�ַ������̽�ⷨ�����ͻ 
		    hashAdress=(++hashAdress)%m; 
		  
		    if (hashTable->elem[hashAdress]==NULLKEY||hashAdress==hash(data)) return -1; 
	  } 
	  return 1;//���ݲ��ҳɹ� 
} 
//����չ�� 
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
//������ 
int main()
{    
      int i,j,result; 
	  HashTable hashTable; 
	  int arr[HASHSIZE]={13,29,27,28,26,30,38}; 
	  
	  printf("***************Hash��ϣ�㷨***************\n"); 
	  
	  //��ʼ����ϣ�� 
	  init(&hashTable); 
	  
	  //�������� 
	  for (i=0;i<HASHSIZE;i++) 
	  { 
	     insert(&hashTable,arr[i]); 
	  } 
	  //��ӡ��Ӧ������ 
	  display(&hashTable); 
	  //���Ҷ�Ӧ������
	  result = search(&hashTable,34);	 
	  if(result == -1){
	  	    printf("�Բ���û���ҵ���\n");
	  	    return -1;
	  }
	  printf("29�ڹ�ϣ���е�λ����:%d\n",result);
	  return 1;
} 

// https://www.cnblogs.com/shuly/p/12198455.html 
