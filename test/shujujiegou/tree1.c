#include <stdio.h>
#include <stdlib.h>
/**
  http://www.manongjc.com/detail/23-sxdvrgseptzimvp.html 来源 
**/ 
/**
  数的节点 
**/
typedef struct node {
	int data;
	struct node *left; //树的左节点
	struct node *right; //树的右节点 
}Node; 

//树的根节点
typedef struct tree{
	Node *root;
}Tree; 

//向树中插入数据
void insert(Tree* tree,int value) 
{
	/*创建一个节点*/
	Node* node = (Node*)malloc(sizeof(Node));
	if(node == NULL){
		 exit("根节点内存分配失败!");
	} 
	node->data = value;
	node->left = NULL;
	node->right = NULL;
	
	//判断树是不是空的
	 
}

