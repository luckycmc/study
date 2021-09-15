package main
import (
	"fmt"
	"net"
	"bufio"
)
//启动tcp服务协成
func tcpProcess(conn net.Conn){
	defer conn.Close()//关闭连接
	for{
		reader := bufio.NewReader(conn)  //获取一个读取对象
		var buf [128]byte
		n, err := reader.Read(buf[:]) // 读取用户发送的数据
		if err != nil {
			fmt.Println("read from client failed, err:", err)
			break
		}
		recvStr := string(buf[:n]) //转换成字符串
		fmt.Println("收到client端发来的数据：", recvStr)
		conn.Write([]byte(recvStr)) //转换成字符串 发送数据给客户端
	}
}
//创建tcp服务端
func main(){
	//1.设置tcp监听端口
	listen,err := net.Listen("tcp","127.0.0.1:20000")//设置监听端口
	if err != nil{
		fmt.Println("listen is failed,err",err)
		return
	}
	//2.建立tcp连接
	for{
		conn,err := listen.Accept() //建立用户连接没有用户连接形成阻塞
		if err != nil{
			fmt.Println("accept failed,err",err)
			 continue //跳出当前循环
		}
		//启动一个协成去处理连接
		go tcpProcess(conn)
	}
}