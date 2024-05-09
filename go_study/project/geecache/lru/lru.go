package lru

import "container/list"

type Cache struct {
	maxBytes int64
	nbytes   int64
	ll       *list.List
	cache    map[string]*list.Element
	//»Øµ÷º¯Êý
	OnEvicted func(key string,value value)
}

type entry struct{
	key string
	value value
}

type value interface{
	Len() int
}