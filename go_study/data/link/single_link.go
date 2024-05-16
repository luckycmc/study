package main

import "fmt"

/**
 主要实现简单的单项链表
**/

/**
  c s是 struct linna{
	   int a
	   lina *next
  }
**/
type LinkNode struct{
	 Len int32            //链表的长度
     Data int64           //链表的数据
	 NextNode *LinkNode   // 链表的下一个节点
}
var  baseNode LinkNode
// 实现链表的曾删改查
// 初始化
func newInitNode(value int) *LinkNode{
	 return &LinkNode{
		 Data: int64(value),
		 NextNode: nil,
	 }
}
//添加节点
func(rootNode *LinkNode) addNode(node *LinkNode){
	
	   //我需要找到最后一个节点 把新的加入
	   newNode := &baseNode
       if newNode.Len == 0 {
		  newNode.NextNode = node
		  newNode.Len++
		  return
	   }
	   fmt.Println(newNode)
       for{
		   if newNode.NextNode == nil {
			  newNode.NextNode = node
			  newNode.Len++
			  break
		   }
		   //一直往下找
		   newNode = newNode.NextNode
	   }
}
//展示节点
func(rootNode *LinkNode) list(firstNode *LinkNode){
	

	for{
		if firstNode.NextNode != nil {
		  
		   //一直往下找
		   firstNode = firstNode.NextNode
		}else{
			break;
		}
		
	}
}

func main(){
    

	d1:=newInitNode(1)
	d2:=newInitNode(2)
	d3:=newInitNode(8)
	
    baseNode.addNode(d1)
	baseNode.addNode(d2)
	baseNode.addNode(d3)

	baseNode.list(&baseNode)
	  
}