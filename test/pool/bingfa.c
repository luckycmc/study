//并发量:服务器承载客户端的数量
// 满足相对应的的客户端需求
// 连接时最基础的条件
// 线程池的操作  1.对IO的操作时间  2.日志的存贮

//post_API

// tcp 和udp的协议
/*
  1.建立连接
  2.传输过程
  3.断开连接

  全连接队列 accept 队列
  半连接队列里面有  并且是最后一次握手  才会进入全连接队列

  accept 等待一个条件的满足  没有满足会进入阻塞的状态
  三次握手完成之后 才进入accept

  半连接状态存在 会进入listen的状态
*/
/**
  1.udp 的缺点就是不稳定
  2.
**/