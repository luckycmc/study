package main
import (
	"fmt"
	"net"
	"io"
)
func main(){
	conn,err := net.Dial("tcp","www.baidu.com:80")
	if err!= nil {
		fmt.Println("连接有误!",err)
		return
	}
	defer conn.Close()
	//发送get请求
	fmt.Fprintf(conn, "GET / HTTP/1.0\r\n\r\n")
	var buf [8192]byte
	//接受相应
	for{
		n,err := conn.Read(buf[:])
		//读完数据终止
		if err==io.EOF {
            return
		}
		//判断是否有错误
		if err != nil{
			fmt.Println("get response failed, err:", err)
			break
		}
		//打印输出相应的数据
		fmt.Print(string(buf[:n]))
	}
}