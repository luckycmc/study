package main

import (
	"fmt"
	"net"
	"strings"
)
//处理对应的数据
func handData(conn net.Conn){
        buf := make([]byte,4096)
        for{
            n,err := conn.Read(buf)
			if err != nil{
				fmt.Println("read data error: ", err)
				return
			}
			if n == 0 {
				fmt.Println("client is close...")
				return
			}
			readMsg := strings.Split(string(buf[:n]),"|")
			//readMsg := string(buf[:n])
			fmt.Println(readMsg)
            /**************拼接发送消息 ack 三次握手 start*****************/
			var msg string
			if readMsg[1] == "ack" {
				msg = "flag|syn+ack|window|64" 
			}else{
                msg = "flag|syn+ack|window|63";
			}
			//fmt.Println(msg)
			_,err1 := conn.Write([]byte(msg))
			if err1 != nil{
				fmt.Println("send data error: ", err1)
				return
			} 
			/**************拼接发送消息 ack 三次握手 end*****************/
			/**************拼接发送消息 eastbalisted start*****************/

			/**************拼接发送消息 eastbalisted start****************/
		}
		//flag := readMsg[1]
		//window := readMsg[3]
		
}
func main() {
       
	    conn, err := net.Listen("tcp", "127.0.0.1:9501")
		if err != nil{
			fmt.Println("Error listening",err)
			return 
		}
        fmt.Println("Listen 127.0.0.1:9501 is running")
		for{
			fd,err:= conn.Accept()
			if err != nil{
				return
			}
			//交个对应协成执行
			go handData(fd);
		}
	  
}