package main
import (
	"fmt"
	"encoding/json"
	"encoding/binary"
	"net"
	"../common/message"
	_"errors"
	"io"
)
//读取客户端发送过来的信息
func readPkg(conn net.Conn) (msg message.Message, err error) {

			buf := make([]byte,4096)
			fmt.Println("读取客户端发送的数据....")
			//conn.Read 在conn没有关闭的情况下，才会阻塞
			//如果客户端关闭了conn 则就不会阻塞了
			_,err = conn.Read(buf[:4])
			if err != nil {
				 //err = errors.New("read pkg header failed")
			     return
		   }
		   //根据buf[:4] 转换成一个uint32类型
		   var pkgLen uint32
		   pkgLen = binary.BigEndian.Uint32(buf[0:4])
		   //更具pkgLen 读取消息内容
		   n,err := conn.Read(buf[:pkgLen])
		   if n != int(pkgLen) ||err != nil {
               // fmt.Println("conn.Read failed err: ", err)
				//err = errors.New("read pkg body failed")
				return
		   }
		   err = json.Unmarshal(buf[:pkgLen],&msg)
		   if err != nil{
			   fmt.Println("json.Unmarshal err: ", err)
			   return
		   }
		   return 
}
//处理与客户端的连接
func process(conn net.Conn)  {
	  
	 defer conn.Close() //延时关闭客户端
	 
	  //读取客户端发送的数据
	  for {
		   mes,err := readPkg(conn)
		   if err != nil {
			   if err == io.EOF {
				    fmt.Println("客户端关闭了连接,服务器端也进行关闭")
					return
			   }else{
				fmt.Println("readPkg err=",err)
				return
			   } 
		   }
		   fmt.Println("mes = ",mes)
	  }  
}
//发送数据
func writePkg(conn net.Conn,data []byte)(err error)  {
	  
	    //先发送一个长度给对端
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
		//发送data数据值
		n,err = conn.Write(data)
		if n != int(pkgLen) ||err != nil {
			
			fmt.Println("conn.Write(bytes) fail",err)
			return
		}
		return
}
//编写一个函数专门处理serviceProcessLogin函数专门处理登录请求
func serviceProcessLogin(conn net.Conn,mes message.Message)(err error)  {
	   
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
		data, err = json.Marshal(loginResMes)
		if err != nil{
			fmt.Println("json.Marshal failed:", err)
			return
		}
		//4将data 赋值给
		resMes.data = string(data)
        //5.对reMes 进行序列化 准备发送
		data, err = json.Marshal(resMes)
		if err != nil{
			fmt.Println("json.Marshal failed:", err)
			return
		}
		//6发送 data
		writePkg(conn,data)
}
//编写一个process函数
//功能:根据客户端发送消息种类的不同,决定用哪个函数处理
func serverProcessMsg(conn net.Conn,mes *message.Message)(err error){
     
	   switch mes.Type {
			case message.LoginMessageType:
				//处理登录的信息
				err = serviceProcessLogin(conn,mes)
			case message.RegisterMesType:
				//处理注册的信息
			default:
				fmt.Println("消息类型不存在无法处理....")
	   }
	   return
}
//主函数
func main()  {
	
	  //提示新消息
	  fmt.Println("服务器在8090端口监听.....")
      //直接进入监听状态
	  listen,err := net.Listen("tcp","0.0.0.0:8889")  //和C语言不同的是C需要 socket bind listen
	  defer listen.Close() //延时关闭客户端
	  if err != nil {
		  fmt.Println("listen failed: ",err)
		  return //程序终止
	  }
	  //一旦监听成功 等待客户端连接服务器
	  for{

		   fmt.Println("等待客户端连接服务器")
		   conn,err := listen.Accept()
		   if err != nil {
			   fmt.Println("listen.Accept err",err)
			   continue  //当前循环跳出
		   }
           
		   //一旦连接成功,则启动一个协成和客户端保持通讯
		    go process(conn)
	  }
}