package process

import(
	"fmt"
	"../../common/message"
	"../model"
	
)
//客户端要维护的map
var onlineUsers map[int]*message.User = make(map[int]*message.User,10)

var CurUser model.CurUser  //在用户登陆成功后 完成对usercuer的初始化

//在客户端显示在线的在线用户
func outputOnlineUsers(){
	//遍历onlineuser 数据
	fmt.Println("当前在线用户列表")
	for id,_ := range onlineUsers{
           //如果不显示自己
		   fmt.Println("用户id:\t",id)

	}
}
//编写一个方法,处理返回的NotifyUserStatusMes
func updateUserStatus(notifyUserStatusMes *message.NotifyUserStatusMes){
      
	   //适当的优化下
	   user,ok := onlineUsers[notifyUserStatusMes.UserId]
	   if !ok {
			user = &message.User{
				UserId:notifyUserStatusMes.UserId,
			}
	   }
       user.UserStatus = notifyUserStatusMes.Status

	   onlineUsers[notifyUserStatusMes.UserId] = user
}

