#include <stdio.h>
#include <stdlib.h>
/**
  http://www.manongjc.com/detail/23-sxdvrgseptzimvp.html ��Դ 
**/ 
/**
  ���Ľڵ� 
**/
typedef struct node {
	int data;
	struct node *left; //������ڵ�
	struct node *right; //�����ҽڵ� 
}Node; 

//���ĸ��ڵ�
typedef struct tree{
	Node *root;
}Tree; 

//�����в�������
void insert(Tree* tree,int value) 
{
	/*����һ���ڵ�*/
	Node* node = (Node*)malloc(sizeof(Node));
	if(node == NULL){
		 exit("���ڵ��ڴ����ʧ��!");
	} 
	node->data = value;
	node->left = NULL;
	node->right = NULL;
	
	//�ж����ǲ��ǿյ�
	 
}

