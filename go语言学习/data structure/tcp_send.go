package main
import (
	"fmt"
	"net"
	"strings"
)

type Client struct {
     
	    status string   //tcp的状态
		window string //     //tcp的窗口
		conn net.Conn
		data string
}

func NewClient()  *Client{
	client := &Client{
		  status:"send",
		  window :"64",
		  data:"syn",
	  }
	  return client
}
//三握手
func (this *Client) hand() bool{
	    
	   conn, err := net.Dial("tcp","127.0.0.1:9501")
	   if err != nil{
		   return false
	   }
	   this.conn = conn
	   msg := "flag|"+this.data+"|window|"+this.window //拼接发送消息
	   _, err2 := conn.Write([]byte(msg))
	   buf := make([]byte, 1024)
	   if err2 != nil{
		   fmt.Println("server 半连接队列是满的")
	   }
	   result, err1 := conn.Read(buf)
	   if err1 != nil{
		  fmt.Println("server 半连接队列是满的")
	   }
	   data := strings.Split(string(buf[:result]),"|")
	   if data[1] == "syn+ack"{
		   return true
	   }
	   return false
}
//发送数据
 func (this *Client)  sendData() {
	fmt.Printf("send data %s\n",this.window)
	for i := 0; i < 10; i++ {
		this.conn.Write([]byte("established|hello world\n"))
		fmt.Println(i)
	}
	
} 
//四次挥手
/* func (this *Client) byeHand() bool  {
	
} */
//主函数
func main()  {
	 
	  client := NewClient()  
	  //三次握手
	  result := client.hand()
	  if result == false { 
		  fmt.Println("hand fail")
		  return
	  }else{
		  fmt.Println("hand to success,so send data")
		  fmt.Println("准备发送数据.................")
		  //发送数据
		  client.sendData()
	  }

	  //发送数据
	  //client.sendData()
     for{

	 }
}