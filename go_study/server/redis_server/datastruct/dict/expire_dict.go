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
func (d ExpireDict) Get(key string) (val interface{}, exists bool) {

	d.mu.Lock()
	val, ok := d.m[key]
	d.mu.Unlock()
	if !ok {
		return nil, ok
	}
	return val, true
}

//设置key

func (d ExpireDict) Put(key string, val interface{}) (result int) {

}
