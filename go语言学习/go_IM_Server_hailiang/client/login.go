package main
import(
	"fmt"
	"encoding/json"
	"encoding/binary"
	"net"
	"../common/message"

)
//写一个函数完成用户表登陆
func login(userId int,passPwd string) (err error) {
	   
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
   //这里还需要服务器端返回的消息
    mes,err = readPkg(conn)  //mes 就是
    if err !=nil {
		fmt.Println("readPkg(conn) err =",err)
		return
	}
	//将mes的data 反序列话 LoginResMes
    var loginResMes message.LoginResMes
	loginResMes,err = json.Unmarshal([]byte(mes.Data))
	if loginResMes.Code == 200{
		fmt.Println("登陆成功")
	}else if loginResMes.Code == 500 {
		fmt.Println(loginResMes.Error)
	}
	return 
}