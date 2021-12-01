package main
import (
	"fmt"
)
/**
    二叉查找树可能退化为链表，也可能是一棵非常平衡的二叉树，查找，添加，删除元素的时间复杂度取决于树的高度 h。

	当二叉树是满的时，树的高度是最小的，此时树节点数量 n 和高度 h 的关系为：h = log(n)。
	当二叉树是一个链表时，此时树节点数量 n 和高度 h 的关系为：h = n。
	二叉查找树的效率来源其二分查找的特征，时间复杂度在于二叉树的高度，因此查找，添加和删除的时间复杂度范围为 log(n)~n。

	为了提高二叉查找树查找的速度，树的高度要尽可能的小。AVL树和红黑树都是相对平衡的二叉查找树，因为特殊的旋转平衡操作，
	树的高度被大大压低。它们查找效率较高，添加，删除，查找操作的平均时间复杂度都为 log(n)，经常在各种程序中被使用。

	二叉查找树是后面要学习的高级数据结构AVL树，红黑树的基础。

**/
//二叉树查找的根节点
type BinarySearchTree struct{
    Root *BinarySearchTreeNode  // 树根节点
}
//对应的节点
type BinarySearchTreeNode struct{
	 
	  Value int64                    //值
	  Times int64                    //该数据出现的次数
	  Left *BinarySearchTreeNode     //左子树
	  Right *BinarySearchTreeNode    //右子数
}

//初始化一个二叉树
func NewBinarySearchTree() *BinarySearchTree {
    return new(BinarySearchTree)
}
//添加元素节点
func (tree *BinarySearchTree) Add(value int64) {
	  
	  //如果没有根数，证明是一个空数,添加根后返回
	  if tree.Root == nil{
		  tree.Root = &BinarySearchTreeNode{Value:value}
		  return 
	  }

	  //添加数据
	  tree.Root.Add(value)
}

    //二叉树添加节点的核心
	func (node *BinarySearchTreeNode) Add(value int64){
		
		//第一次肯定要和根节点比较
		if value < node.Value {   //大于根节点
			
			// 如果插入的值比节点的值小，那么要插入到该节点的左子树中
			// 如果左子树为空，直接添加
			if node.Left == nil {
				node.Left = &BinarySearchTreeNode{Value: value}
			} else {
				// 否则递归
				node.Left.Add(value)
			}
		} else if value > node.Value{   //小于根节点
				// 如果插入的值比节点的值大，那么要插入到该节点的右子树中
				// 如果右子树为空，直接添加
				
				if node.Right == nil {
					node.Right = &BinarySearchTreeNode{Value: value}
				} else {
					// 否则递归
					node.Right.Add(value)
				}  
		}else{  //等于根节点
				// 值相同，不需要添加，值出现的次数加1即可
			node.Times = node.Times + 1
		}
	}
     // 找出最小值的节点
	func (tree *BinarySearchTree) FindMinValue() *BinarySearchTreeNode {
		  
		if tree.Root == nil {
			// 如果是空树，返回空
			return nil
		}
		return tree.Root.FindMinValue()
	}

	func (node *BinarySearchTreeNode) FindMinValue() *BinarySearchTreeNode {
		// 左子树为空，表面已经是最左的节点了，该值就是最小值
		if node.Left == nil {
			return node
		}
        fmt.Println(node.Value)
		// 一直左子树递归
		return node.Left.FindMinValue()
	}
	// 找出最大值的节点
	func (tree *BinarySearchTree) FindMaxValue() *BinarySearchTreeNode {
		if tree.Root == nil {
			// 如果是空树，返回空
			return nil
		}

		return tree.Root.FindMaxValue()
	}

	func (node *BinarySearchTreeNode) FindMaxValue() *BinarySearchTreeNode {
		// 右子树为空，表面已经是最右的节点了，该值就是最大值
		if node.Right == nil {
			return node
		}

		// 一直右子树递归
		return node.Right.FindMaxValue()
	}

	// 查找指定节点
	func (tree *BinarySearchTree) Find(value int64) *BinarySearchTreeNode {
		if tree.Root == nil {
			// 如果是空树，返回空
			return nil
		}

		return tree.Root.Find(value)
	}

	func (node *BinarySearchTreeNode) Find(value int64) *BinarySearchTreeNode {
		if value == node.Value {
			// 如果该节点刚刚等于该值，那么返回该节点
			return node
		} else if value < node.Value {
			// 如果查找的值小于节点值，从节点的左子树开始找
			if node.Left == nil {
				// 左子树为空，表示找不到该值了，返回nil
				return nil
			}
			return node.Left.Find(value)
		} else {
			// 如果查找的值大于节点值，从节点的右子树开始找
			if node.Right == nil {
				// 右子树为空，表示找不到该值了，返回nil
				return nil
			}
			return node.Right.Find(value)   //一直递归查找
		}
	}
	// 查找指定节点的父亲
	func (tree *BinarySearchTree) FindParent(value int64) *BinarySearchTreeNode {
		if tree.Root == nil {
			// 如果是空树，返回空
			return nil
		}

		// 如果根节点等于该值，根节点其没有父节点，返回nil
		if tree.Root.Value == value {
			return nil
		}
		return tree.Root.FindParent(value)
	}

	func (node *BinarySearchTreeNode) FindParent(value int64) *BinarySearchTreeNode {
		// 外层没有值相等的判定，因为在内层已经判定完毕后返回父亲节点。

		if value < node.Value {
			// 如果查找的值小于节点值，从节点的左子树开始找
			leftTree := node.Left
			if leftTree == nil {
				// 左子树为空，表示找不到该值了，返回nil
				return nil
			}

			// 左子树的根节点的值刚好等于该值，那么父亲就是现在的node，返回
			if leftTree.Value == value {
				return node
			} else {
				return leftTree.FindParent(value)
			}
		} else {
			// 如果查找的值大于节点值，从节点的右子树开始找
			rightTree := node.Right
			if rightTree == nil {
				// 右子树为空，表示找不到该值了，返回nil
				return nil
			}

			// 右子树的根节点的值刚好等于该值，那么父亲就是现在的node，返回
			if rightTree.Value == value {
				return node
			} else {
				return rightTree.FindParent(value)
			}
		}
	}

	// 删除指定的元素
	func (tree *BinarySearchTree) Delete(value int64) {
		if tree.Root == nil {
			// 如果是空树，直接返回
			return
		}

		// 查找该值是否存在
		node := tree.Root.Find(value)
		if node == nil {
			// 不存在该值，直接返回
			return
		}

		// 查找该值的父亲节点
		parent := tree.Root.FindParent(value)

		// 第一种情况，删除的是根节点，且根节点没有儿子
		if parent == nil && node.Left == nil && node.Right == nil {
			// 置空后直接返回
			tree.Root = nil
			return
		} else if node.Left == nil && node.Right == nil {
			//  第二种情况，删除的节点有父亲节点，但没有子树

			// 如果删除的是节点是父亲的左儿子，直接将该值删除即可
			if parent.Left != nil && value == parent.Left.Value {
				parent.Left = nil
			} else {
				// 删除的原来是父亲的右儿子，直接将该值删除即可
				parent.Right = nil
			}
			return
		} else if node.Left != nil && node.Right != nil {
			// 第三种情况，删除的节点下有两个子树，因为右子树的值都比左子树大，那么用右子树中的最小元素来替换删除的节点，这时二叉查找树的性质又满足了。

			// 找右子树中最小的值，一直往右子树的左边找
			minNode := node.Right
			for minNode.Left != nil {
				minNode = minNode.Left
			}
			// 把最小的节点删掉
			tree.Delete(minNode.Value)

			// 最小值的节点替换被删除节点
			node.Value = minNode.Value
			node.Times = minNode.Times
		} else {
			// 第四种情况，只有一个子树，那么该子树直接替换被删除的节点即可

			// 父亲为空，表示删除的是根节点，替换树根
			if parent == nil {
				if node.Left != nil {
					tree.Root = node.Left
				} else {
					tree.Root = node.Right
				}
				return
			}
			// 左子树不为空
			if node.Left != nil {
				// 如果删除的是节点是父亲的左儿子，让删除的节点的左子树接班
				if parent.Left != nil && value == parent.Left.Value {
					parent.Left = node.Left
				} else {
					parent.Right = node.Left
				}
			} else {
				// 如果删除的是节点是父亲的左儿子，让删除的节点的右子树接班
				if parent.Left != nil && value == parent.Left.Value {
					parent.Left = node.Right
				} else {
					parent.Right = node.Right
				}
			}
		}
	}

	// 中序遍历
	func (tree *BinarySearchTree) MidOrder() {
		tree.Root.MidOrder()
	}

	func (node *BinarySearchTreeNode) MidOrder() {
		if node == nil {
			return
		}

		// 先打印左子树
		node.Left.MidOrder()

		// 按照次数打印根节点
		for i := 0; i <= int(node.Times); i++ {
			fmt.Println(node.Value)
		}

		// 打印右子树
		node.Right.MidOrder()
	}
	//主函数
	func main() {
		values := []int64{15,20, 9, 8, 6, 9, 2,3, 5, 40, 7, 9, 13, 6, 83, 6, 8}
	
		// 初始化二叉查找树并添加元素
		tree := NewBinarySearchTree()
		for _, v := range values {
			tree.Add(v)
		}
	
		// 找到最大值或最小值的节点
		fmt.Println("find min value:", tree.FindMinValue().Value)
        fmt.Println("find max value:", tree.FindMaxValue())

		// 查找不存在的99
		node := tree.Find(99)
		if node != nil {
			fmt.Println("find it 99!")
		} else {
			fmt.Println("not find it 99!")
		}

		// 查找存在的9
		node = tree.Find(9)
		if node != nil {
			fmt.Println("find it 9!")
		} else {
			fmt.Println("not find it 9!")
		}

		// 删除存在的9后，再查找9
		tree.Delete(9)
		node = tree.Find(9)
		if node != nil {
			fmt.Println("find it 9!")
		} else {
			fmt.Println("not find it 9!")
		}

		// 中序遍历，实现排序
		tree.MidOrder()
   }