package main

import (
	"fmt"
	"os"
)
//定义Emp
type Emp struct {
	Id int
	Name string
	Next *Emp
}
//法法待定

//定义EmLink
type EmpLink struct {
	Head *Emp
}
//添加雇员方法
func (this *EmpLink) Insert(emp *Emp)  {
	cur:=this.Head  //辅助指针
	var pre *Emp = nil //辅助指针pre 在cur前面

	//如果当前的EmpLink是一个空链表
	if cur == nil{
		this.Head = emp //完成初始化
		return
	}
	//如果是一个空链表给emp找到对应的位置插入
	//思路 让cur和emp比较,然后pre 保持在cur的前面
	for{
		if cur != nil{
			//比较
			if cur.Id >= emp.Id{
				//找到了
				break
			}
			//保证同步
			pre = cur
			cur = cur.Next
		}else{
			break   //找到了退出来
		}
	}
	//退出时，我们看下是否将emp添加到链表最后
	pre.Next = emp
	emp.Next = cur
}
//显示当前链表的信息
func (this *EmpLink) ShowLink(no int)  {
	if  this.Head == nil{

		  fmt.Printf("链表%d 为空\n",no)
		  return
	}
	//遍历当前的链表显示当前链表的数据
	//首先需要一个辅助指针
	cur := this.Head
	for{
		if cur != nil {
           fmt.Printf("链表%d 雇主id=%d 名字 = %s\t",no,cur.Id,cur.Name)
		   //节点后移
		   cur = cur.Next
		}else{
			break   //退出
		}
		fmt.Println()
	}

}
//定义hashtable，链表数组
type HashTable struct {
	LinkArr [7]EmpLink
}
//给hashtable 编写Insert雇员方法
func (this *HashTable) Insert(emp *Emp)  {

	  //根据id确定雇员添加到那个链表
	 linkNo := this.HashFun(emp.Id)
	 //使用对应的链表添加
	 this.LinkArr[linkNo].Insert(emp)
}

//编写一个方法显示所有的雇员
func (this *HashTable) ShowAll()  {

	for i:=0 ;i<len(this.LinkArr);i++{
		this.LinkArr[i].ShowLink(i)
	}
}
//编写一个散列方法
func (this *HashTable) HashFun(id int) int {

	  return id %7  //得到一个值 对应链表的	下表
}
//编写一个方法完成查询
func (this *HashTable) FindById(id int) *Emp  {
	//使用散列表,确定该雇员在那个链表
	linkNo := this.HashFun(id)
	return this.LinkArr[linkNo].FindById(id)
}
//根据id查找对应的雇员,如果没有就返回nil
func (this *EmpLink) FindById(id int)  *Emp {
     cur:=this.Head
	for  {
		if cur!=nil && cur.Id == id {
            return cur
		}else if cur == nil {
			break
		}
		//节点后移
		cur = cur.Next
	}
	return nil
}
//主函数
func main()  {
    var key string
    var id int
	var name string
	var hashtable HashTable
	for  {
		fmt.Println("=========雇员系统菜单==========")
		fmt.Println("input 表示添加雇员")
		fmt.Println("show  表示显示雇员")
		fmt.Println("find  表示查找雇员")
		fmt.Println("exit  表示退出系统")
		fmt.Println("请输入你的选着>>:")
		fmt.Scan(&key)
		switch key {
			case "input":
				fmt.Println("请输入雇员id>>")
				fmt.Scan(&id)
				fmt.Println("请输入雇主姓名>>")
				fmt.Scan(&name)
				emp := &Emp{
					Id:id,
					Name: name,
				}
				hashtable.Insert(emp)
			case "show":
				hashtable.ShowAll()
			case "find": //查找
                 fmt.Println("请输入id号:")
				 fmt.Scan(&id)
                 emp := hashtable.FindById(id)
				if emp == nil {
                    fmt.Printf("id=%d 的雇员不存在\n\t",id)
				}else {
                     
				}
			case "exit":
				os.Exit(-1)
			default:
				fmt.Println("你输入错误")
			
		}
	}
}