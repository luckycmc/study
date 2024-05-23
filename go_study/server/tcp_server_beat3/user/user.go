package user

import (
	"context"
	"fmt"
	"tcp_server_beat3/model"
	"time"
)

type User struct{}
const userLoginKey = "u:l:k"
// 注册用户
func (this User) Register(data *model.User) bool{
      return model.SaveUser(data)
}

//用户登录
func(this User) Login(data *model.User) bool{
	 ctx := context.Background()
	 res,_ := model.RedisDb.Get(ctx,userLoginKey).Result()
	 fmt.Println("redis get:",res)
	 
	 if res =="" {
		   
		   //用户登录 
		   s:= model.Login(data)
		   if s {
			//设置数据到 redis中
			    err:=model.RedisDb.Set(ctx,userLoginKey,data.PassWord+data.UserName,time.Second *10).Err()
				if err != nil {
					 fmt.Println("redis set is error",err)
				}
			    return s
		   }
		   return false
	 }
	 
	 return true
	
}
func (this User) FindFriend() {

}