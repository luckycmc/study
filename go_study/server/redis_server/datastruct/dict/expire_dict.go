package dict

import (
	"sync"
	"time"
)

// 带时间类型的数据处理
type ExpireDict struct {
	m        map[string]interface{}
	mu       sync.Mutex
	expirt   time.Time
	dictType string //字符串 lits hash 等等
}

func MakeExpireDict() *ExpireDict {

	return &ExpireDict{
		m:  make(map[string]interface{}),
		mu: sync.Mutex{},
	}
}

// 获取key
func (d *ExpireDict) Get(key string) (val interface{}, exists bool) {

	d.mu.Lock()
	val, ok := d.m[key]
	d.mu.Unlock()
	if !ok {
		return nil, ok
	}
	return val, true
}

//设置key

func (d *ExpireDict) Put(key string, val interface{}) (result int) {

	_, ok := d.m[key]
	if ok {
		return 0
	}
	//确保数据的安全性
	d.mu.Lock()
	d.m[key] = val
	d.mu.Lock()
	return 1
}
//当一个key 存在时不进行 设置设置返回失败 要设置枷锁
func(d *ExpireDict) PutIfAbsent(key string,val interface{}) int{

	  defer  d.mu.Unlock()
	  d.mu.Lock()
	  _,ok:=d.m[key]
	  if ok {
		 return 0;
	  }
      d.m[key] = val
	  return 1
}
// PutIfExists puts value if the key is exist and returns the number of inserted key-value
// 对存在的值进行更新
func(d *ExpireDict) PutIfExists(key string,val interface{}) int{
	
	 d.mu.Lock()
     _,ok:= d.m[key]
	 d.mu.Unlock()
	 if ok {
		 d.m[key] = val
		 return 1
	 }
	 return 0
}
// 获取当前字典的个数
func (d *ExpireDict) Len() int {
	if d.m == nil {
		return 0
	}
	return len(d.m)
}
