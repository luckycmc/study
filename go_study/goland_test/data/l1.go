package main

import (
	"fmt"
	"runtime"
)

// 定义一个结构体
type LinkNode struct {
	Data     int64     //对应的数据
	NextNode *LinkNode //对应的数据节点
}

func main() {
	runtime.NumCPU()
	// 新的节点
	node := new(LinkNode)
	node.Data = 2

	// 新的节点
	node1 := new(LinkNode)
	node1.Data = 3
	node.NextNode = node1 // node1 指向下一个节点的指针 node

	// 新的节点
	node2 := new(LinkNode)
	node2.Data = 4
	node1.NextNode = node2 // node2 链接到 node1 节点上

	// 按顺序打印数据
	nowNode := node
	for {
		if nowNode != nil {
			// 打印节点值
			fmt.Println(nowNode.Data)
			// 获取下一个节点
			nowNode = nowNode.NextNode
		} else {
			// 如果下一个节点为空，表示链表结束了
			break
		}
	}
	fmt.Println("link node is end")
}
