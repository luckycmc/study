package process

import(
	"fmt"
	"os"
	"net"
	"../utils"
	"../../common/message"
	"encoding/json"
)

//显示登陆后的界面.....
func ShowMenu(){
	fmt.Println("-----------恭喜XXXx登录成功-------")
	fmt.Println("-----------1.显示在线用户列表------")
	fmt.Println("-----------2.发行消息-------------")
	fmt.Println("-----------3.信息列表-------------")
	fmt.Println("-----------4.退出系统-------------")
	fmt.Println("请选着(1-4):")
	var key int
	var content string

	smsProcess := &SmsProcess{

	}

	fmt.Scanf("%d\n",&key)
	switch key{
		case 1:
			//fmt.Println("显示在线用户类表-")
			outputOnlineUsers()
		case 2:
			fmt.Println("请输入你想对大家说什么:")
            fmt.Scanf("%s\n",&content)
            smsProcess.SendGroupMes(content)
		case 3:
			fmt.Println("信息列表-")
		case 4:
			fmt.Println("你选择退出了系统-")
			os.Exit(0)
		default:
			fmt.Println("你输入的选项不对...")
	}
}
//和服务器端保持通讯
func serverProcessMes(conn net.Conn){
      
	   //创一个一个transfer实例,不停地读取服务端发送的消息
	   tf := &utils.Transfer{
		    Conn:conn,
	   }
	   for{
		   fmt.Println("客户端正在读取服务器端发送的消息")
		   mes,err := tf.ReadPkg()
		   if err !=nil {
			   fmt.Println("tf.ReadPkg err = ",err)
			   return 
		   }
		   //如果读取到消息则下一个处理
            switch mes.Type {
			      case message.NotifyUserStatusMesType :  //有人上线了
				  //1.取出notifymessage
                  var notifyUserStatusMes message.NotifyUserStatusMes
				  json.Unmarshal([]byte(mes.Data),&notifyUserStatusMes)
				  //2.这个用户的状态保存到map客户端
				  updateUserStatus(&notifyUserStatusMes)
			default:
				fmt.Println("服务器端反回了一个未知消息")
			}
		   //fmt.Println("mes=%v",mes)
	   }
}