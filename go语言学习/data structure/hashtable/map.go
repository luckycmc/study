package main
import(
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

