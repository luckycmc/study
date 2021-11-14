package model

import(
	"fmt"
	"github.com/garyburd/redigo/redis"
	"encoding/json"
	"../../common/message"
)
//我们在服务器启动的时候,就初始化 一个UserDao s实例
var (
	MyUserDao *UserDao
)
//定义一个UserDao 结构体
//玩成对应user结构体的各种操作

type UserDao struct{
	pool *redis.Pool
}
//使用工厂模式,创建一个UserDao实例
func NewUserDao(pool *redis.Pool)(userDao *UserDao){
    
	   userDao = &UserDao{
            pool:pool,
	   }
	   return
}
//根据用户id获取用户的信息
func(this *UserDao) getUserById(conn redis.Conn,id int) (user *User,err error){
      
	   //更具用户id 去redis 
	   res,err := redis.String(conn.Do("HGet","users",id))
	   if err!= nil{
		   //错误提示
		   if err == redis.ErrNil{  //表示users 哈希表中没有找到对应的id
			     
			   err = ERROR_USER_NOTEXISTS
		   }
		   return 
	   }
	   //这里需要吧res反序列化User实例
	   err = json.Unmarshal([]byte(res),&user)
	   if err !=nil{
		   fmt.Println("json err",err)
	   }
	   return
}  
//完成登陆的校验LOgin
func (this *UserDao) Login(userId int,userPwd string) (user *User,err error)  {
	   
	   //先从UserDao的连接池中取出一个连接出来
	   conn := this.pool.Get()
	   defer conn.Close()
	   user,err = this.getUserById(conn,userId)
	   if err !=nil {
		   return
	   }

	   //验证该用户
	   if user.UserPwd != userPwd {
		   err = ERROR_USER_PWD
		   return
	   }
	   return
}

//完成登陆的校验Register
func (this *UserDao) Register(user *message.User) (err error)  {
	   
	//先从UserDao的连接池中取出一个连接出来
	conn := this.pool.Get()
	defer conn.Close()
	_,err = this.getUserById(conn,user.UserId)
	if err ==nil {
		 err = ERROR_USER_EXISTS
		return
	}
    //说明redis还没有
	data,err := json.Marshal(user)
	if err != nil {
		return
	}
	//数据入库
	_,err =conn.Do("Hset","users",user.UserId,string(data))
    if err != nil {
		fmt.Println("保存用户注册错误 err = ",err)
		return
	}
	return
}