package process2

import(
	"fmt"
	"net"
	"../../common/message"
	"encoding/json"
    "../utils"
	"../model"
)

type UserProcess struct{
	//分析它的字段
	Conn net.Conn   //当前 用户的连接
	//增加一个字段
	UserId int      //对应的当前用户的id
}
//服务注册
func(this *UserProcess) ServiceProcessRegister(mes *message.Message)(err error)  {
     //1.先从mes中取出mes.Data,并直接反序列化LoginMes
	var registerMes message.RegisterMes
	err = json.Unmarshal([]byte(mes.Data), &registerMes)
	if err != nil {
		fmt.Println("json.Unmarshal failed error: ", err)
		return
	}

	//1先声明一个resMes
	var resMes message.Message
	resMes.Type = message.RegisterMesType

	//2在声明一个LoginResMes
	var registerResMes message.RegisterResMes

	err = model.MyUserDao.Register(&registerMes.User)
    if err != nil{
		 
		  if err == model.ERROR_USER_EXISTS {
		    
			registerResMes.Code  = 505
			registerResMes.Error = model.ERROR_USER_EXISTS.Error()
		  }else{
            
			registerResMes.Code  = 506
			registerResMes.Error = "注册发生未知错误...."
		  }
	}else{
		registerResMes.Code  = 200
	}

	data,err := json.Marshal(registerResMes)
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
    
	user,err := model.MyUserDao.Login(loginMes.UserId,loginMes.UserPwd)
	fmt.Println("某人....",user)
    if err !=nil {
        
		if err == model.ERROR_USER_NOTEXISTS {
			loginResMes.Code  = 500
			//先测试成功 然后再根据 具体的信息返回对应的状态信息
			loginResMes.Error = err.Error()
		}else if err == model.ERROR_USER_PWD {
			loginResMes.Code  = 300
			//先测试成功 然后再根据 具体的信息返回对应的状态信息
			loginResMes.Error = err.Error()
		}else{
			loginResMes.Code  = 505
			//先测试成功 然后再根据 具体的信息返回对应的状态信息
			loginResMes.Error = "服务器内部错误"
		}
	}else{
		loginResMes.Code = 200
		//这里是用户登录成功,我们就把登录的信息放入到userMgr中

		this.UserId = loginMes.UserId //插入对应的id
		userMgr.AddOnlineUsers(this)
		//讲当前在线用户的id 存放到 loginResMes.UsersId
		//遍历userMgr.onlineUsers
		for id,_ := range userMgr.onlineUsers{
			loginResMes.UsersId = append(loginResMes.UsersId,id)  //追加到对应的切片里面
		}
		fmt.Println(user,"登陆成功!")
	}
	//如果用户的id为100 密码等于123456 认为合法 否者不合法
	/*if loginMes.UserId == 100 && loginMes.UserPwd == "123456" {
		  
			//合法
			loginResMes.Code = 200
			loginResMes.Error = "用户登录成功"
	}else{
		   //合法
			loginResMes.Code  = 500
			loginResMes.Error = "该用户不存在请注册在使用"
	}*/
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