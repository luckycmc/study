package lru

import "container/list"

type Cache struct {
	maxBytes int64  //����ڴ�
	nbytes   int64  // �Ѿ�ʹ���˶���
	ll       *list.List
	cache    map[string]*list.Element    //list��һ��˫������
	//�ص�����
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

 //���ҹ���
 func(c *Cache) Get(key string)(value Value,ok bool){
        
	    //���������Ƿ����
		if ele,ok := c.cache[key];ok{
			c.ll.MoveToFront(ele)   //�������Ӧ������ڵ���ڣ��򽫶�Ӧ�ڵ��ƶ�����β�������ز��ҵ���ֵ
			kv :=ele.Value.(*entry) 
			return kv.value,true
		}
	    return
 }

 //�Ƴ��ɵ� ʵ���Ͼ��ǻ�����̭
 func(c *Cache) RemoveOldest(){
	  ele  := c.ll.Back()   //ȡ�����е��׽ڵ�
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
 //���ӻ��߸���
 func(c *Cache) Add(key string,value Value){
       
	    //�ж��Ƿ����  ������ ���� �����������
	    if ele,ok := c.cache[key];ok{
              //�����ݽڵ������ƶ�
			  c.ll.MoveToFront(ele)
			  //��ȡ����
			  kv:= ele.Value.(*entry)
			  c.nbytes += int64(value.Len()) - int64(kv.value.Len())
		      kv.value = value
		}else {
			ele := c.ll.PushFront(&entry{key, value})
			c.cache[key] = ele
			c.nbytes += int64(len(key)) + int64(value.Len())
		}	
        //ִ���ڴ���̭����
		for c.maxBytes != 0 && c.maxBytes < c.nbytes {
			c.RemoveOldest()
		}
 }  

 // Len the number of cache entries
func (c *Cache) Len() int {
	return c.ll.Len()
}