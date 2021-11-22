package process

import(
	"fmt"
	"../utils"
	"../../common/message"
	"encoding/json"
)

type SmsProcess struct{

}

//发送群发消息
func (this *SmsProcess) SendGroupMes(content string) (err error) {
	  
	   //1.创建一个mes
	   var mes message.Message
	   mes.Type =  message.SmsMesType

	   // 2.创建一个SmsMes实例
	   var smsMes message.SmsMes

	   smsMes.Content    = content //对应的内容
	   smsMes.UserId     = CurUser.UserId   //当前用户的userid
	   smsMes.UserStatus = CurUser.UserStatus  //当前用户的状态

	   //将数据序列化
	   data,err := json.Marshal(smsMes)
	   if err != nil {
		   fmt.Println("SendGroupMes json.Marshal",err)
		   return
	   }
	   mes.Data = string(data)
	   //对mes  
	   data,err = json.Marshal(mes)

	   if err != nil {
		   fmt.Println("SendGroupMes json.Marshal",err)
		   return
	   }

	   //5.将序列化的数据发送
	   tf := &utils.Transfer{
		   Conn:CurUser.Conn,
	   }

	   //6.发送
	   err = tf.WritePkg(data)
	   if err != nil {
		   fmt.Println("SendGroupMes error",err)
		   return
	   }
	   
    return
}
