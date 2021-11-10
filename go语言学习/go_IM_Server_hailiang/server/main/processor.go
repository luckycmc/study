package main

import (
	"fmt"
	"net"
	"../../common/message"
	"io"
	"../utils"
	"../process2"
)
//先创建一个process结构体
type Processor struct{
	Conn net.Conn
}
//功能:根据客户端发送消息种类的不同,决定用哪个函数处理
func(this *Processor) ServerProcessMsg(mes *message.Message)(err error){
     
	switch mes.Type {
		 case message.LoginMessageType:
			 //处理登录的信息
			 //创建一个实例
			 up := &process2.UserProcess{
				 Conn:this.Conn,
			 }
			 err = up.ServiceProcessLogin(mes)
		 case message.RegisterMesType:
			 //处理注册的信息
		 default:
			 fmt.Println("消息类型不存在无法处理....")
	}
	return
}
//process2 方法读写数据
func (this *Processor) Process2()(err error)  {
	  //读取客户端发送的数据
	for {
		//创建一个transfer 的实例完成读包的任务
		//创建一个实例
		tf := &utils.Transfer{
			Conn :this.Conn,
		}
		mes,err := tf.ReadPkg()
		if err != nil {
			if err == io.EOF {
				 fmt.Println("客户端关闭了连接,服务器端也进行关闭")
				 return err
			}else{
			 fmt.Println("readPkg err=",err)
			 return err
			} 
		}
		//fmt.Println("mes = ",mes)
		//传入到对应的操作
		err = this.ServerProcessMsg(&mes)
		if err != nil{
			return err
		}
  }  
}