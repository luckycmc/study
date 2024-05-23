package user

import "tcp_server_beat3/model"

type User struct{}

// 注册用户
func (this User) Register(data *model.User) bool{
      return model.SaveUser(data)
}

func (this User) FindFriend() {

}