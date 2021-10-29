package main
import (
	"fmt"
	"net"
)

type Client struct {
     
	    status string   //tcp的状态
		window int      //tcp的窗口
		conn net.Conn
		data string
}

func createClient()  *Client{
	 
	  client := &Client{
		  status:"send",
		  window :64,
		  data:"syn"
	  }
}
//三握手
func (this *Client) hand() bool{
	
}
//发送数据
func (this *Client)  sendData() int {
	
}
//四次挥手
func (this *Client) byeHand() bool  {
	
}
//主函数
func main()  {
	
}