package dict

import "sync"

type SyncDict struct {
	m sync.Map
}

func MakeSyncDict() *SyncDict {

	return &SyncDict{}
}

// 获取key 的值
func (dict *SyncDict) Get(key string) (val interface{}, exists bool) {

	val, ok := dict.m.Load(key)
	return val, ok

}

// 获取当前数据库下有几个key
func (dict *SyncDict) Len() int {
	length := 0
	dict.m.Range(func(key, value any) bool {
		length++
		return true
	})
	return length
}

// 设置key val
func (dict *SyncDict) Put(key string, val interface{}) (result int) {
	_, exists := dict.m.Load(key)
	dict.m.Store(key, val)
	if exists {
		return 0
	}
	return 1
}

// 不存在设置
func (dict *SyncDict) PutIfAbsent(key string, val interface{}) (result int) {
	_, existed := dict.m.Load(key)
	if existed {
		return 0
	}
	dict.m.Store(key, val)
	return 1
}

// 判断key 是否存在
func (dict *SyncDict) PutIfExists(key string, val interface{}) (result int) {
	_, existed := dict.m.Load(key)
	if existed {
		dict.m.Store(key, val)
		return 1
	}
	return 0
}

// 移除key
func (dict *SyncDict) Remove(key string) (result int) {
	_, existed := dict.m.Load(key)
	dict.m.Delete(key)
	if existed {
		return 1
	}
	return 0
}

// 获取当前字典下的所有key
func (dict *SyncDict) Keys() []string {
	result := make([]string, dict.Len())
	i := 0
	dict.m.Range(func(key, value any) bool {
		result[i] = key.(string)
		i++
		return true
	})
	return result
}

// 遍历字典的所有key
func (dict *SyncDict) ForEach(consumer Consumer) {
	dict.m.Range(func(key, value interface{}) bool {
		consumer(key.(string), value)
		return true
	})
}

// 随机获取几个key
func (dict *SyncDict) RandomKeys(limit int) []string {
	result := make([]string, limit)
	for i := 0; i < limit; i++ {
		dict.m.Range(func(key, value interface{}) bool {
			result[i] = key.(string)
			return false
		})
	}
	return result
}

// 随机获取不重复的key
func (dict *SyncDict) RandomDistinckKeys(limit int) []string {
	result := make([]string, limit)
	i := 0
	dict.m.Range(func(key, value interface{}) bool {
		result[i] = key.(string)
		i++
		if i == limit {
			return false
		}
		return true
	})
	return result
}

// 清空当前字典下的所有key
func (dict *SyncDict) Clear() {
	*dict = *MakeSyncDict()
}
