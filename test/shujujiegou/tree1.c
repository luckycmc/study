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
		return ; 
	} 
	node->data = value;
	node->left = NULL;
	node->right = NULL;
	
	//�ж����ǲ��ǿյ�
	 if (tree->root == NULL){
	 	 
	 	 tree->root = node;
	 }else{
	 	Node *temp = tree->root; /*��������ʼ*/
	 	while(temp != NULL){
	 		
	 		  if(value <temp->data){  /*С�ھͽ������*/
	 		  	     //���������Ľڵ� 
	 		  	     if(temp->left == NULL){
	 		  	     	temp->left = node;
	 		  	     	return;
					}else{
						temp = temp->left;// ����������� 
					} 
			   }else{ /*������Ҷ���*/
			   	   if(temp->right == NULL)
	                {
	                    temp->right = node;
	                    return;
	                }
	                else /*�����ж�*/
	                {
	                    temp = temp->right;
	                }
			   }
		 } 
	 }
}

/*
 ����һ������
 �������:����������
 */
void traverse(Node* node)
{
    if(node != NULL)
    {
        traverse(node->left);
        printf("%d ",node->data);
        traverse(node->right);
    }
}

/*����һ����*/
void distory_tree(Node* node)
{
    
    if(node != NULL)
    {
        distory_tree(node->left);
        distory_tree(node->right);
        printf("free node:%d\n",node->data);
        free(node);
        node = NULL;
    }
}
/*������*/
int main()
{
    int i = 0;
    Tree tree;
    tree.root = NULL;/*����һ������*/
    int n;
    printf("input total num:\n");
    /*����n���������������*/
    scanf("%d",&n);
    for(i = 0; i < n; i++)
    {
        int temp;
        scanf("%d",&temp);
        insert(&tree, temp);
    }
    /*����������*/
    traverse(tree.root);
    
    /*����һ����*/
    distory_tree(tree.root);
    return 0;
}

