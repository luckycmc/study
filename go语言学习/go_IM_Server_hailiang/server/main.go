package main
import (
	"fmt"
	"encoding/json"
	"encoding/binary"
	"net"
	"../common/message"
	_"errors"
	"io"
)
//读取客户端发送过来的信息
func readPkg(conn net.Conn) (msg message.Message, err error) {

			buf := make([]byte,4096)
			fmt.Println("读取客户端发送的数据....")
			//conn.Read 在conn没有关闭的情况下，才会阻塞
			//如果客户端关闭了conn 则就不会阻塞了
			_,err = conn.Read(buf[:4])
			if err != nil {
				 //err = errors.New("read pkg header failed")
			     return
		   }
		   //根据buf[:4] 转换成一个uint32类型
		   var pkgLen uint32
		   pkgLen = binary.BigEndian.Uint32(buf[0:4])
		   //更具pkgLen 读取消息内容
		   n,err := conn.Read(buf[:pkgLen])
		   if n != int(pkgLen) ||err != nil {
               // fmt.Println("conn.Read failed err: ", err)
				//err = errors.New("read pkg body failed")
				return
		   }
		   err = json.Unmarshal(buf[:pkgLen],&msg)
		   if err != nil{
			   fmt.Println("json.Unmarshal err: ", err)
			   return
		   }
		   return 
}
//处理与客户端的连接
func process(conn net.Conn)  {
	  
	 defer conn.Close() //延时关闭客户端
	 
	  //读取客户端发送的数据
	  for {
		   mes,err := readPkg(conn)
		   if err != nil {
			   if err == io.EOF {
				    fmt.Println("客户端关闭了连接,服务器端也进行关闭")
					return
			   }else{
				fmt.Println("readPkg err=",err)
				return
			   } 
		   }
		   fmt.Println("mes = ",mes)
	  }  
}
func main()  {
	
	  //提示新消息
	  fmt.Println("服务器在8090端口监听.....")
      //直接进入监听状态
	  listen,err := net.Listen("tcp","0.0.0.0:8889")  //和C语言不同的是C需要 socket bind listen
	  defer listen.Close() //延时关闭客户端
	  if err != nil {
		  fmt.Println("listen failed: ",err)
		  return //程序终止
	  }
	  //一旦监听成功 等待客户端连接服务器
	  for{

		   fmt.Println("等待客户端连接服务器")
		   conn,err := listen.Accept()
		   if err != nil {
			   fmt.Println("listen.Accept err",err)
			   continue  //当前循环跳出
		   }
           
		   //一旦连接成功,则启动一个协成和客户端保持通讯
		    go process(conn)
	  }
}