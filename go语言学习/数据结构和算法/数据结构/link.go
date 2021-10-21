package main
import (
	"fmt"
)

//定义节点
type Node struct{
	Value int
	Next *Node
	Size int
}

//头节点初始化 
var head = new(Node)

//给链表添加节点
func addNode(t *Node,v int) int{
     
	  if head == nil{
		  t = &Node{v,nil,1}  //初始化节点 也就是第一个节点 所以 t.Size的值为1
		  head = t
		  return 0
	  }
	  for t != nil{
           //该节点是否存在
		if v == t.Value{
			fmt.Println("节点已存在:", v)
			return -1
		}
		t.Size++
        
		t=t.Next  //节点后移
	  }
	   //找到对应的空节点复制
	  t.Next = &Node{v,nil,t.Size}
	  return -2
}
// 遍历链表
func traverse(t *Node){
	  //判断节点是否为空
	  if t == nil{
		  fmt.Println("-> 空链表!")
		  return 
	  }
      for t != nil{
		fmt.Printf("%d -> ", t.Value)
        t = t.Next //指针节点后移
	  }
	  fmt.Println() //输出空格
}
// 查找节点
func lookupNode(t *Node,v int) bool{
        
	if head == nil {
        t = &Node{v, nil,0}
        head = t
        return false
    }
	for t != nil{
        //找到了
		if v == t.Value {
			return true
		}
		t = t.Next
	}
	return false
}
// 获取链表长度
func size(t *Node) int{
	if t == nil {
        fmt.Println("链表的长度-> 空链表!")
        return 0
    }
	return t.Size
}
// 入口函数
func main() {
    fmt.Println(head)
    head = nil
    // 遍历链表
    traverse(head) 
    // 添加节点 
    addNode(head, 1)
    addNode(head, -1)
    // 再次遍历
    traverse(head)
    // 添加更多节点
    addNode(head, 10)
    addNode(head, 5)
    addNode(head, 45)
    // 添加已存在节点
    addNode(head, 5)
	//打印节点的长度
	fmt.Println("链表的长度是:",size(head))
    // 再次遍历
    traverse(head)
   // 查找已存在节点
    if lookupNode(head, 5) {
        fmt.Println("该节点已存在!")
    } else {
        fmt.Println("该节点不存在!")
    }
   // 查找不存在节点 
    if lookupNode(head, -100) {
        fmt.Println("该节点已存在!")
    } else {
        fmt.Println("该节点不存在!")
    }
}