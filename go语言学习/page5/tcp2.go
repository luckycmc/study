package main

import(
	"fmt"
	"net"
	"bufio"
)

//协成处理函数
func process(conn net.Conn)  {
	
	  defer conn.Close()  //关闭客户端的连接

	  //读取客户端发送过来的数据
	  for {

		    reader := bufio.NewReader(conn)
			var buf[128]byte
			//读取数据
			n,err := reader.Read(buf[:])
			if err != nil {
				fmt.Println("read from client failed, err:", err)
				break
			}
			//数据转换
			recvStr := string(buf[:n])
			fmt.Println("recv from client data:",recvStr)
			//数据返回给客户端
			conn.Write([]byte(recvStr))
	  }
}

func main() {
	 
	  fmt.Println("tcp server is startting")
	  //直接进入监听状态
	  listen,err := net.Listen("tcp","0.0.0.0:8090")  //和C语言不同的是C需要 socket bind listen
	  if err != nil {
		  fmt.Println("listen failed: ",err)
		  return //程序终止
	  }
	  //一直接受客户端连接 7 * 24 小时
	  for{

		     //tcp 进入accept 队列
			 conn,err := listen.Accept()  //完成三次握手建立连接
			 if err != nil {
				fmt.Println("accept failed, err:", err)
				continue  //结束当前的连接  进入下一个连接
			 }
			 //启动一个go协成完成客户端的业务
			 go process(conn)
	  }
}