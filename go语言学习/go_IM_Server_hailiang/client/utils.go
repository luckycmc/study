package main

import (
	"fmt"
	"encoding/json"
	"encoding/binary"
	"net"
	"../common/message"
	_"io"
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

//发送数据
func writePkg(conn net.Conn,data []byte)(err error)  {

	//先发送一个长度给对端
	//先获取到 data的长度->转成一个表示长度的byte切片
	var pkgLen uint32
	pkgLen = uint32(len(data))
	var bytes [4]byte
	binary.BigEndian.PutUint32(bytes[0:4], pkgLen)
	//发送长度
	n,err := conn.Write(bytes[0:4])
	if n !=4 ||err != nil {
		
		fmt.Println("conn.Write(bytes) fail",err)
		return
	}
	//发送data数据值
	n,err = conn.Write(data)
	if n != int(pkgLen) ||err != nil {
		
		fmt.Println("conn.Write(bytes) fail",err)
		return
	}
   return
}
