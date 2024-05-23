package user

import "tcp_server_beat3/model"

type User struct{}

// 注册用户
func (this User) Register(data *model.User) bool{
      return model.SaveUser(data)
}

//用户登录
func(this User) Login(data *model.User) bool{
	//用户登录
     return model.Login(data)
}
func (this User) FindFriend() {

}