package main

import "fmt"
import "net"
import "bufio"
//协成处理函数
func process(conn net.Conn){
      defer conn.Close() //关闭连接
	  //读取客户端的数据
	  for{
		reader := bufio.NewReader(conn)
		var buf [128]byte
		n,err := reader.Read(buf[:])// 读取数据
		//客户端关闭
		if n== 0{
			fmt.Println("client is closed");
			break
		}
		if err != nil{
			fmt.Println("read from client failed, err:", err)
            break
		}
		recvStr := string(buf[:n]) //转换字符串
		fmt.Println("收到client端发来的数据：", recvStr)
		conn.Write([]byte(recvStr)) //发送数据
	  }
}

func main()   {
	listen,err := net.Listen("tcp","127.0.0.1:8080")
	if err != nil {
		fmt.Println("listen failed,err:",err)
         return
 	}

	//监听客户端的连接
	for{
		conn, err := listen.Accept() //接受客户端的连接
		if err != nil {
			fmt.Println("accept failed,err:",err)
			continue
		}
		//启动一个协成出路客户端的fd
		go  process(conn)
	}
}