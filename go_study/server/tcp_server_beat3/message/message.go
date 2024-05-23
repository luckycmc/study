package message

import (
	"fmt"
	"strings"
	"tcp_server_beat3/model"
)

type Message struct{
	model.User
	 Type string
}

//解析字符串
func  ParseMessage(data string) *Message{
      
	  fmt.Println(data)

	  newMesg := strings.Split(data, "|")
	  userName := strings.Split(newMesg[0],":")[1] //获取用户名
	  passWord := strings.Split(newMesg[1],":")[1] //获取用户名
	  userType := strings.Split(newMesg[2],":")[1] //用户操作的类型

	  /*fmt.Println(userName)
	  fmt.Println(passWord)
	  fmt.Println(userType)*/

	  return &Message{
		  User: model.User{
			UserName: userName,
			PassWord: passWord,
		  },
		  Type: userType,
	  }
}

