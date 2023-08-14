package main

import(
	"fmt"
	"net"
	"flag"
)

type Client struct{
	ServerIp string
	ServerPort int
	Name string
	conn net.Conn  //连接对象
	flag int
}

//创建客户端对象
func NewClient(serverIp string,serverPort int) *Client{
   
     //创建client对象
     client := &Client{
     	ServerIp:serverIp,
     	ServerPort:serverPort,
     	flag:99,
     }

     //创建连接
     conn,err := net.Dial("tcp",fmt.Sprintf("%s:%d",serverIp,serverPort))
     if err != nil {
     	fmt.Println("dail connect is err",err)
     	return nil
     }
     
      client.conn = conn
      return client
}
//处理主业务
func(client *Client) Run(){
    
     for client.flag != 0{
     	 for client.menu() != true{

     	 }
     	 switch client.flag{
     	 case 1:
     	 	//公聊模式
     	 	fmt.Println("1")
     	 	break
     	 case 2:
     	 	fmt.Println("2")
     	 	break
     	 case 3:
     	 	fmt.Println("3")
     	 	break
     	 }
     }
}
//显示菜单
func(client *Client) menu() bool{
   
    var flag int
    fmt.Println("1.公聊模式")
    fmt.Println("2.私聊模式")
    fmt.Println("3.更新用户名")
    fmt.Println("0.退出")

    //获取 客户端参数
    fmt.Scanln(&flag)

    if flag >=0 && flag <=3{
        
         client.flag = flag
         return true
    }else{
    	fmt.Println(">>>>>请输入合法范围内的数字<<<<<<")
    	return false
    }
}
var serverIp string
var serverPort int
 
//./client -ip 127.0.0.1 -port 8888
func init() {
    flag.StringVar(&serverIp, "ip", "127.0.0.1", "设置服务器IP地址(默认是127.0.0.1)")
    flag.IntVar(&serverPort, "port", 8888, "设置服务器端口(默认是8888)")
}

func main(){
       //命令行解析
       flag.Parse()
      //连接客户端
      client := NewClient(serverIp,serverPort)
      if client == nil {
      	fmt.Println("服务器链接失败>>>>>")
      	return 
      }
      fmt.Println("服务器连接成功>>>>>")

      //启动客户端的业务
      client.Run()
}