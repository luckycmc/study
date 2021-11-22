package process2

import(
	"fmt"
	"encoding/json"
	"../../common/message"
	"../utils"
	"net"
)

type SmsProcess struct{

	   //暂时不需要字段
}

func (this *SmsProcess) SendGroupMes(mes *message.Message)  {
       
	  //取出mes的内容SmsMes
	  var smsMes message.SmsMes
	  err := json.Unmarshal([]byte(mes.Data),&smsMes)
	  if err !=nil {
		  fmt.Println("json.Unmarshal err=",err)
		  return
	  }
	  data,err := json.Marshal(mes)
	  if err != nil {
		  fmt.Println("json.Marshal",err)
		  return
	  }
	  //遍历服务器onlineUsers map[int]*UserProcess,
	  //将消息转发取出
	  for _,up := range userMgr.onlineUsers{
          
		//这里还需要过滤到自己
		//if id == 
		 this.SendMesToEachOnlineUser(data,up.Conn)
	  }
}

//发送给每一个用户
func (this *SmsProcess) SendMesToEachOnlineUser(data []byte,conn net.Conn)  {
	 
	  tf := &utils.Transfer{
		  Conn:conn,
	  }
	  err := tf.WritePkg(data)
	  if err != nil {
		  fmt.Println("转发消息失败",err)
	  }
}