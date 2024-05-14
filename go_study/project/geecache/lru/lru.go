package lru

import "container/list"

type Cache struct {
	maxBytes int64   //最大内存
	nbytes   int64   // 已经使用了多少
	ll       *list.List
	cache    map[string]*list.Element    //list是一个双向链表
	//回调函数
	OnEvicted func(key string,value Value)
}

type entry struct{
	key string
	value Value
}

type Value interface{
	Len() int
}

// New is the Constructor of Cache
func New(maxBytes int64,OnEvicted func(string,Value)) *Cache{

	  return &Cache{
		  maxBytes: maxBytes,
		  ll: list.New(),
		  cache: make(map[string]*list.Element),
		  OnEvicted: OnEvicted,
	  }
}

 //查找功能
 func(c *Cache) Get(key string)(value Value,ok bool){
        
	    //查找数据是否存在
		if ele,ok := c.cache[key];ok{
			c.ll.MoveToFront(ele)   //如果键对应的链表节点存在，则将对应节点移动到队尾，并返回查找到的值
			kv :=ele.Value.(*entry) 
			return kv.value,true
		}
	    return
 }

 //移除旧的 实际上就是缓存淘汰
 func(c *Cache) RemoveOldest(){
	  ele  := c.ll.Back()   //取出队列的首节点
	  if ele != nil {
		   
		    c.ll.Remove(ele)
			kv := ele.Value.(*entry)
			delete(c.cache,kv.key)
			c.nbytes-=int64(len(kv.key)) + int64(kv.value.Len())
			if c.OnEvicted != nil {
				  c.OnEvicted(kv.key,kv.value)
			}
	  }
 }
 //增加或者更新
 func(c *Cache) Add(key string,value Value){
       
	    //判断是否存在  存在择 更新 不存在择添加
	    if ele,ok := c.cache[key];ok{
              //把数据节点往后移动
			  c.ll.MoveToFront(ele)
			  //获取数据
			  kv:= ele.Value.(*entry)
			  c.nbytes += int64(value.Len()) - int64(kv.value.Len())
		      kv.value = value
		}else {
			ele := c.ll.PushFront(&entry{key, value})
			c.cache[key] = ele
			c.nbytes += int64(len(key)) + int64(value.Len())
		}	
        //执行内存淘汰策略
		for c.maxBytes != 0 && c.maxBytes < c.nbytes {
			c.RemoveOldest()
		}
 }  

 // Len the number of cache entries
func (c *Cache) Len() int {
	return c.ll.Len()
}