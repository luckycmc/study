package model

import(
	"fmt"
	"github.com/garyburd/redigo/redis"
)

//定义一个UserDao 结构体
//玩成对应user结构体的各种操作

type UserDao struct{
	pool *redisPool
}

func(this *UserDao) getUserById(conn redis.Conn,id int) (user *User,err error){
      
	   //更具用户id 去redis 
	   res,err := redis.String(conn.Do("HGet","users",id))
	   if err!= nil{
		   //错误提示
		   if err == redis.ErrNil{  //表示users 哈希表中没有找到对应的id
			     
			   err = ERRORU_USER_NOTEXISTS
		   }
		   return 
	   }
}  