package process2
import(
	"fmt"
	"net"
	"../../common/message"
	"encoding/json"
    "../utils"
)
type UserProcess struct{
	//分析它的字段
	Conn net.Conn
}
//编写一个函数专门处理serviceProcessLogin函数专门处理登录请求
func(this *UserProcess) ServiceProcessLogin(mes *message.Message)(err error)  {
	   
	//1.先从mes中取出mes.Data,并直接反序列化LoginMes
	var loginMes message.LoginMes
	err = json.Unmarshal([]byte(mes.Data), &loginMes)
	if err != nil {
		fmt.Println("json.Unmarshal failed error: ", err)
		return
	}
	//1先声明一个resMes
	var resMes message.Message
	resMes.Type = message.LoginMessageType

	//2在声明一个LoginResMes
	var loginResMes message.LoginResMes

	//如果用户的id为100 密码等于123456 认为合法 否者不合法
	if loginMes.UserId == 100 && loginMes.UserPwd == "123456" {
		  
			//合法
			loginResMes.Code = 200
			loginResMes.Error = "用户登录成功"
	}else{
		   //合法
			loginResMes.Code  = 500
			loginResMes.Error = "该用户不存在请注册在使用"
	}
	//3.将loginMessage 序列化
	data,err := json.Marshal(loginResMes)
	if err != nil{
		fmt.Println("json.Marshal failed:", err)
		return
	}
	//4将data 赋值给
	resMes.Data = string(data)
	//5.对reMes 进行序列化 准备发送
	data,err = json.Marshal(resMes)
	if err != nil{
		fmt.Println("json.Marshal failed:", err)
		return
	}
	tf := &utils.Transfer{
		Conn :this.Conn,
	}
	//6发送 data
	err = tf.WritePkg(data)
	return
}