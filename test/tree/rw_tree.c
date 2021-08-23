#include <stdio.h>

typedef int KEY_VALUE;
/**
 * 红黑树节点
 * */
typedef struct _rbtree_node{
     
        unsigned char color;
        struct _rbtree_node *left;
        struct _rbtree_node *right;
        struct _rbtree_node *parent;

        //定义数据节点
        KEY_VALUE key;
        void *value;

}rbtree_node;  

typedef struct _rbtree{
     
       struct _rbtree_node *root;
       struct _rbtree_node *nil;

}rbtree;