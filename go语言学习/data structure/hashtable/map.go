package main

import (
	"fmt"
	"sync"
	"unsafe"
)

//集合结构体
type Set struct{
	 m     map[int]struct{}    //用字典来实现
	 len   int                  //集合的大小
	 sync.RWMutex               //读写锁实现线程安全
}

//初始化一个空集合
func NewSet(cap int64) *Set{

	  temp := make(map[int]struct{},cap)
	  return &Set{
		  m:temp,
	  }
}

//给集合添加一个元素
func (s *Set) Add(item int)  {
	   s.Lock()
	   defer s.Unlock()
       //添加集合的元素
	   s.m[item] = struct{}{}
	   //更改集合的长度
	   s.len = len(s.m)
}
//移除集合中的元素
func (s *Set) Remove(item int){
      
	  //集合中没有元素直接返回
	  if s.len == 0 {
          fmt.Println("集合为空")
		  return 
	  }

	  s.Lock()
	  delete(s.m,item)
	  s.Unlock()

	  s.len = len(s.m)  // 重新计算元素数量
}
//查看集合中是否存在某个元素
func(s *Set) Has(item int ) bool{
	s.Lock()
	defer s.Unlock()
	_,ok := s.m[item]
	return ok
}

//查看结合的大小
func(s *Set) Len() int{
	return s.len
}
//清除集合中的所有元素
func (s *Set) Clear()  {
    s.Lock()
	defer s.Unlock()
	s.m = map[int]struct{}{}  //字典重新复制
	s.len = 0
}
//判断集合是否时空
func (s *Set) IsEmpty() bool  {
	if s.Len() == 0{
		return true
	}
	return false
}

//将集合转换为链表
func (s *Set) List() []int  {

	  s.RLock()
	  defer s.RUnlock()
	  list := make([]int,0,s.len)
	for item := range s.m{
         list = append(list,item)
	}
	return list
}

// 为什么使用空结构体
func other() {
	a := struct{}{}
	b := struct{}{}
	if a == b {
		fmt.Printf("right:%p\n", &a)
	}

	fmt.Println(unsafe.Sizeof(a))
}

func main() {
	//other()

	// 初始化一个容量为5的不可重复集合
	s := NewSet(5)

	s.Add(1)
	s.Add(1)
	s.Add(2)
	fmt.Println("list of all items", s.List())

	s.Clear()
	if s.IsEmpty() {
		fmt.Println("empty")
	}

	s.Add(1)
	s.Add(2)
	s.Add(3)

	if s.Has(2) {
		fmt.Println("2 does exist")
	}

	s.Remove(2)
	s.Remove(3)
	fmt.Println("list of all items", s.List())
}
