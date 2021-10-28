package main
import (
	"net"
	"fmt"
	"flag"
	"io"
	"os"
)
//客户端结构体
type Client struct{
     ServerIp string
	 ServerPort int
	 Name string
	 conn net.Conn
	 flag int   //当前client 的模式
}

//创建客户端方法
func NewClient(serverIp string, serverPort int) *Client { 
	  
	  //创建客户端
	  client := &Client{
		  ServerIp: serverIp,
		  ServerPort: serverPort,
		  flag :9999,
	  }
      
	  //连接server 拨号 连接服务器
	  conn, err := net.Dial("tcp",fmt.Sprintf("%s:%d",serverIp,serverPort))
	  if err != nil {
		  fmt.Println("net.Listen err:",err)
		  return nil
	  }
      client.conn = conn
	  //返回对象
	  return client
}
//处理server回应的消息 直接显示到标准输出即可
func (client *Client) DealResponse(){
	  
	  //一旦client.conn有数据 就直接copy到stdout标准输出上，永久阻塞监听
	  io.Copy(os.Stdout, client.conn)
}
//菜单
func (client *Client) menu() bool { 
     
	   var flag int
	   fmt.Println("1.公聊模式")
	   fmt.Println("2.私聊模式")
	   fmt.Println("3.更新用户名")
	   fmt.Println("0.退出")

	   fmt.Scanln(&flag)

	   if flag >= 0 && flag <=3{
		   client.flag = flag
		   return true
	   }else {
           
		   fmt.Println(">>>> 请输入合法范围内的数字<<<<<")
		   return false
	   }
}
//查询用户
func (client *Client) SelectUsers()  {
	  
	   sendMsg := "who\n"
	   _,err := client.conn.Write([]byte(sendMsg))
	   if err != nil {
		   fmt.Println("conn Write error: ", err)
		   return
	   }
}
//私聊模式
func (client *Client) PrvilateChat()  {
	   
	   var remoteName string
	   var chatMsg string
	   client.SelectUsers()
	   fmt.Println(">>>>请输入聊天[用户名],exit退出")
	   fmt.Scanln(&remoteName)

	   for remoteName != "exit" {
		    fmt.Println(">>>>请输入聊天内容,exit退出")
            fmt.Scanln(&chatMsg)
	   }

}
//公聊模式
func (client *Client) PublicChat()  {
	  
	   //提示用户输入消息
	   var chatMsg string
	   fmt.Println(">>>>请输入聊天内容,exit退出")
       fmt.Scanln("%s", &chatMsg)
	   //只要不退出就接受用户消息
	   for chatMsg != "exit" {
              
		      //发送消息
			  if len(chatMsg) !=0 {
				  
				   sendMsg := chatMsg+"\n"
				   _,err := client.conn.Write([]byte(sendMsg))
				   if err != nil {
					   fmt.Println("conn Write err:",err)
					   break
				   }
			  }
			  
			  chatMsg =""  //消息重置为空
			  fmt.Println(">>>>请输入聊天内容,exit退出")
              fmt.Scanln(&chatMsg)
	   }
}
//更新用户名
func (client *Client) UpdateName() bool {
	  
	   fmt.Println(">>>>>请输入用户名:")
	   fmt.Scanln(&client.Name)
	   sendMsg := "rename|"+client.Name+"\n"
	   _,err := client.conn.Write([]byte(sendMsg))
	   if err != nil {
		    fmt.Println("conn.Write err:",err)
			return false
	   }
	   return true
}
//主业务
func (client *Client) run(){
     
	  for client.flag != 0{

		   for client.menu() != true{

		   }
		   //更具不同的模式处理不同的业务
		   switch client.flag{
		        case 1:
					//公聊模式
					client.PublicChat()
					break
			    case 2:
					fmt.Println("私聊模式选择...")
					//私聊模式
					break
				case 3:
					//fmt.Println("更新用户名选择")
					//修改用户名
					client.UpdateName()
					break
		   }
	  }
}

var serverIp    string
var serverPort  int

// 客户端 ./client -ip 127.0.0.1 
// init 初始化函数
func init() {
      
	  flag.StringVar(&serverIp, "ip","127.0.0.1", "设置服务器IP地址(默认是127.0.0.1)")
	  flag.IntVar(&serverPort, "port",9501,"服务器默认端口是8888")
}

func main() {
       
	   flag.Parse()  //解析命令行

	   client := NewClient(serverIp,serverPort)
	   if client == nil {
		   fmt.Println(">>>>>>服务器连接失败....")
		   return
	   }
	   //单独开启一个goroutine 去处理server的回执消息
	   go client.DealResponse()

	   fmt.Println(">>>>>>服务器连接成功......")

	   //启动客户端业务
	   client.run()
}