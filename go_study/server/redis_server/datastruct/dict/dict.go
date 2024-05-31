package dict

/**
@字典相关的处理
**/
type Consumer func(key string, val interface{}) bool

type Dict interface {
	Get(key string) (val interface{}, exists bool)
	Len() int
	Put(key string, val interface{}) (result int)
	PutIfAbsent(key string, val interface{}) (result int)
	PutIfExists(key string, val interface{}) (result int)
	Remove(key string) (result int)
	Foreach(consumer Consumer)
	Keys() []string
	RandomKeys(limit int) []string
	RandomDistinckKeys(limit int) []string
	clear()
}