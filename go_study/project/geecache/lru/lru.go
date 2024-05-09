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

// New is the Constructor of Cache
func New(maxBytes int64,OnEvicted func(string,value)) *Cache{

	  return &Cache{
		  maxBytes: maxBytes,
		  ll: list.New(),
		  cache: make(map[string]*list.Element),
		  OnEvicted: OnEvicted,
	  }
}

