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
		return ; 
	} 
	node->data = value;
	node->left = NULL;
	node->right = NULL;
	
	//判断树是不是空的
	 if (tree->root == NULL){
	 	 
	 	 tree->root = node;
	 }else{
	 	Node *temp = tree->root; /*从树根开始*/
	 	while(temp != NULL){
	 		
	 		  if(value <temp->data){  /*小于就进左儿子*/
	 		  	     //遍历了最后的节点 
	 		  	     if(temp->left == NULL){
	 		  	     	temp->left = node;
	 		  	     	return;
					}else{
						temp = temp->left;// 持续向左边找 
					} 
			   }else{ /*否则进右儿子*/
			   	   if(temp->right == NULL)
	                {
	                    temp->right = node;
	                    return;
	                }
	                else /*继续判断*/
	                {
	                    temp = temp->right;
	                }
			   }
		 } 
	 }
}

/*
 遍历一整颗树
 中序遍历:先左后根再右
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

/*销毁一棵树*/
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
/*主函数*/
int main()
{
    int i = 0;
    Tree tree;
    tree.root = NULL;/*创建一个空树*/
    int n;
    printf("input total num:\n");
    /*输入n个数并创建这个树*/
    scanf("%d",&n);
    for(i = 0; i < n; i++)
    {
        int temp;
        scanf("%d",&temp);
        insert(&tree, temp);
    }
    /*遍历整个树*/
    traverse(tree.root);
    
    /*销毁一棵树*/
    distory_tree(tree.root);
    return 0;
}

