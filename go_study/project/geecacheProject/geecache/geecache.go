package geecache

import (
	"fmt"
	"log"
	"sync"
)

// 负责与外部交互，控制缓存存储和获取的主流程

type Getter interface{
	 Get(key string)([]byte,error)
}

//定义一个回调函数
type GetterFunc func(key string) ([]byte,error)

func (f GetterFunc) Get(key string)([]byte,error){
	 return f(key)
}
//数据结构 Group
type Group struct{
	  name string
	  getter Getter
	  mainCache cache
}

var (
	mu sync.RWMutex  //读写锁
	groups = make(map[string]*Group)
)
//初始化group
func NewGroup(name string,cacheBytes int64,getter Getter) *Group{
     
	    if getter == nil {
			  
			   panic("nil Getter")
		}
		mu.Lock()
		defer mu.Unlock()
		g := &Group{
			name: name,
			getter: getter,
			mainCache: cache{cacheBytes: cacheBytes},
		}
		groups[name] = g
		return g
}

//获取group
func GetGroup(name string) *Group{
         
	    //获取用哪个读锁
	    mu.RLock()
		g := groups[name]    
		mu.RUnlock()
		return g
}
//获取数据
func(g *Group) Get(key string)(ByteView,error){
	 
	   //  如果 key 为空
	   if key == "" {
		    return ByteView{},fmt.Errorf("key is required")
	   }  

	   if v,ok := g.mainCache.get(key);ok {
		     
		   log.Panicln("[GeeCache] hit")
		   return v,nil
	   }

	   return g.load(key)
}
//load 调用 getLocally（分布式场景下会调用 getFromPeer 从其他节点获取）
func (g *Group) load(key string) (value ByteView, err error) {
	return g.getLocally(key)
}
//获取本地数据
func (g *Group) getLocally(key string) (ByteView, error) {
	bytes, err := g.getter.Get(key)
	if err != nil {
		return ByteView{}, err

	}
	value := ByteView{b: cloneBytes(bytes)}
	g.populateCache(key, value)
	return value, nil
}
//将源数据添加到缓存 mainCache
func (g *Group) populateCache(key string, value ByteView) {
	g.mainCache.add(key, value)
}