package main
import (
	"fmt"
	"net"
)
func main(){
	listen,err := net.ListenUDP("udp",&net.UDPAddr{
		IP: net.IPv4(0,0,0,0),
		Port : 30000,
	})
	if err != nil{
		fmt.Println("listen failed,err:",err)
		return
	}
	defer listen.Close()
	//循环接受吧数据
	for{
		var data [1024]byte
		//监听发送过来的数据
		n,addr,err := listen.ReadFromUDP(data[:])//接受数据
		if err != nil {
			fmt.Println("read udp failed, err:", err)
			continue
		}
		//打印接受到的数据
		fmt.Printf("data:%v addr:%v count:%v\n", string(data[:n]), addr, n)
		//发送数据原路返回
		_, err = listen.WriteToUDP(data[:n], addr) // 发送数据
		if err != nil {
			fmt.Println("write to udp failed, err:", err)
			continue
		} 
	}
}