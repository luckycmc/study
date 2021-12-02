package main

import(
	"fmt"
)

// AVL树
type AVLTree struct {
    Root *AVLTreeNode // 树根节点
}

// AVL节点
type AVLTreeNode struct {
    Value  int64                 // 值
    Times  int64                 // 值出现的次数
    Height int64                 // 该节点作为树根节点，树的高度，方便计算平衡因子
    Left   *AVLTreeNode // 左子树
    Right  *AVLTreeNode // 右字树
}

// 初始化一个AVL树
func NewAVLTree() *AVLTree {
    return new(AVLTree)
}

func main(){

}