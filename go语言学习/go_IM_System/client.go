package main
import (
	"net"
	"fmt"
	"flag"
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
      
	  //连接server
	  conn, err := net.Dial("tcp",fmt.Sprintf("%s:%d",serverIp,serverPort))
	  if err != nil {
		  fmt.Println("net.Listen err:",err)
		  return nil
	  }
      client.conn = conn
	  //返回对象
	  return client
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
//主业务
func (client *Client) run(){
     
	  for client.flag != 0{

		   for client.menu() != true{

		   }
		   //更具不同的模式处理不同的业务
		   switch client.flag{
		        case 1:
					//公聊模式
					fmt.Println("公聊模式选择...")
					break
			    case 2:
					fmt.Println("私聊模式选择...")
					//私聊模式
					break
				case 3:
					fmt.Println("更新用户名选择")
					//修改用户名
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
	   fmt.Println(">>>>>>服务器连接成功......")

	   //启动客户端业务
	   client.run()
}