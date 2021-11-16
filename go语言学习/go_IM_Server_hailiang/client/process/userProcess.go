package process

import(
	"fmt"
	"encoding/json"
	"encoding/binary"
	"net"
	"../../common/message"
	"../utils"
	"os"
)
//UserProcess 结构体
type UserProcess struct{
     //暂时不需要字段....
}
//用户注册
func (this *UserProcess) Register(userId int,userPwd string,userName string)(err error)  {
	   //连接到服务器上
	conn, err := net.Dial("tcp","localhost:8889")
	if err != nil {
		 fmt.Println("net.Dial err=",err)
		 return 
	}
	//延时关闭
	defer conn.Close()
	//准备通过当前的conn 发送数据给服务器
	var msg message.Message
	msg.Type = message.RegisterMesType
	//3.创建一个LoginMes 结构体
	var registerMsg message.RegisterMes

	registerMsg.User.UserId = userId
	registerMsg.User.UserPwd = userPwd
	registerMsg.User.UserName = userName

	//将register序列化
	data,err := json.Marshal(registerMsg)
	if err != nil{
		fmt.Println("json.Marshal err = ",err)
		return
	}
	//吧data 赋予给mes.Data 字段
	msg.Data = string(data)
	data,err = json.Marshal(msg)
	if err != nil{
		fmt.Println("json.Marshal err = ",err)
		return
	}

	//创建一个Transfer 实例
	tf := &utils.Transfer{
		Conn:conn,
	}
	//发送data 给服务端
	err = tf.WritePkg(data)
	if err != nil{
		 fmt.Println("注册发送信息出错了")
	}
	//读取数据
	msg,err = tf.ReadPkg()  //mes 就是 register相应
	if err !=nil {
		fmt.Println("readPkg(conn) err =",err)
		return
	}

	//将mes的data 反序列话 RegisterResMes
	var registerResMes message.RegisterResMes
	err = json.Unmarshal([]byte(msg.Data),&registerResMes)
	//判断最好的状态
	if registerResMes.Code == 200{
		fmt.Println("注册成功,请重新登陆一把")
        os.Exit(0)
	}else {
		fmt.Println(registerResMes.Error)
		os.Exit(0)
	}
	return 
}
//写一个函数完成用户表登陆
func(this *UserProcess) Login(userId int,passPwd string) (err error) {
	   
	//下一个就要开始定义协议
	//fmt.Printf(" userId = %d userPwd = %s\n",userId,passPwd)

	//return nil

	//连接到服务器上
	conn, err := net.Dial("tcp","localhost:8889")
	if err != nil {
		 fmt.Println("net.Dial err=",err)
		 return 
	}
	//延时关闭
	defer conn.Close()
	//准备通过当前的conn 发送数据给服务器
	var msg message.Message
	msg.Type = message.LoginMessageType
	//3.创建一个LoginMes 结构体
	var loginMsg message.LoginMes

	loginMsg.UserId = userId
	loginMsg.UserPwd = passPwd

	//4.讲loginMsg 序列化
	data,err := json.Marshal(loginMsg)
	if err != nil {
		fmt.Println("json.Marshal error:", err)
		return 
	}
	//5.吧data赋值给了mes.Data的字段
	msg.Data = string(data)
	//6.将mes 进行序列化
	data,err = json.Marshal(msg)
	if err != nil {
	  fmt.Println("json.Marshal error:", err)
	  return 
  }
  //7.data就是我们发送的数据
  //7.1先把data的长度发送给服务器
  //先获取到 data的长度->转成一个表示长度的byte切片
  var pkgLen uint32
  pkgLen = uint32(len(data))
  var bytes [4]byte
  binary.BigEndian.PutUint32(bytes[0:4], pkgLen)
  //发送长度
  n,err := conn.Write(bytes[0:4])
  if n !=4 ||err != nil {
		
		fmt.Println("conn.Write(bytes) fail",err)
		return
  }
  //fmt.Println("客户端发送消息长度ok！")
  //发送消息体本身
  _,err = conn.Write(data)
  if n !=4 ||err != nil {
		
	  fmt.Println("conn.Write(data) fail",err)
	  return
 }
 //这里还需要服务器端返回的消息 客户端接收的数据
 // 创建一个Transfer 实例
  tf := &utils.Transfer{
	  Conn:conn,
  }
  //读取数据
  msg,err = tf.ReadPkg()  //mes 就是
  if err !=nil {
	  fmt.Println("readPkg(conn) err =",err)
	  return
  }
  
  //将mes的data 反序列话 LoginResMes
  var loginResMes message.LoginResMes
  err = json.Unmarshal([]byte(msg.Data),&loginResMes)
  //判断最好的状态
  if loginResMes.Code == 200{
	  //fmt.Println("登陆成功")
	  //可以显示当前用户的列表,遍历里面的id
	  fmt.Println("当前用户列表如下:")
	  for _,v := range loginResMes.UsersId{
		   if v == userId{  //过滤自己
			   continue
		   }
           fmt.Println("用户id:",v)
	  }
	  fmt.Println()
	  fmt.Println()
	  //在这里还需要启动一个协成保持和客户端的一个通讯
	  //如果服务器有你推送 给客户端 接受并显示客户端的地方
	  go serverProcessMes(conn)

	  //1.显示登录成功后的菜单
	  for{
		ShowMenu()
	  }
  }else {//登录信息错误

	  fmt.Println(loginResMes.Error)
  }
  return 
}