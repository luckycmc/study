package main
import(
	"fmt"
)
//二叉树结构体的定义
type TreeNode struct{
	Data string         //对应的数据
	Left  *TreeNode    //左节点
	Right *TreeNode
}
// 先序遍历
func PreOrder(tree *TreeNode) {
	if tree == nil {
		return
	}

	// 先打印根节点
	fmt.Println(tree.Data, " ")
	// 再打印左子树
	PreOrder(tree.Left)
	// 再打印右字树
	PreOrder(tree.Right)
}
func main(){
	t := &TreeNode{Data: "A"}
	t.Left = &TreeNode{Data: "B"}  //给树的左节点赋值B
	t.Right = &TreeNode{Data:"C"}
	t.Left.Left = &TreeNode{Data: "D"}  //给树的左节点的的左节点 赋值给D
	t.Left.Right = &TreeNode{Data: "E"}  //给树的左节点的的左节点 赋值给D
	t.Right.Left = &TreeNode{Data:"H"}
	fmt.Println("先序排序：")
	PreOrder(t)
}
