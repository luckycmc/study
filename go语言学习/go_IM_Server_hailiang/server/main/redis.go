package main
import (
    _"fmt"
	"time"
	"github.com/garyburd/redigo/redis"
)
//定义一个全局pool
var pool *redis.Pool

func initPool(address string,maxIdle,maxActive int,idleTimeout time.Duration)  {
	  
	   pool = &redis.Pool{
            
		     MaxIdle:maxIdle,//最大空闲连接数
			 MaxActive:maxActive,//最大连接数
			 IdleTimeout:idleTimeout,  //最大空闲时间
			 Dial:func() (redis.Conn,error)  {  //初始化连接代码
				  return redis.Dial("tcp",address)
			 },
	   }
}