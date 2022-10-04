epoll:
   //创建epoll 
   epoll_create(int size);
  // size  没有什么意仅供内核参考
  //返回值 ： 值得是创建红黑树的根节点fd
     //s失败：-1 error

    epoll_ctl(int epfd,int op,int fd,struct epoll_event *event);  //对红黑树进程操作  用来控制操作的
      //1.epfd epoll_create 创建的返回值，代表着那个树的根节点
      //2.op  EPOLL_DEL,EPOLL_CTL_ADD,等属性操作 监听对应的事件
      //3,fd 代表对应操作的fd
      //4.event 结构体地址， events事件 EPOLLIN/EPOLLOUT/EPOLLOUT    
      // data  联合体  int fd 对应监听事件的fd  void *ptr 泛型指针
    

    int  epoll_wait(int epfd,struct epoll_event *events,int maxevents,int timeout); //返回对应的操作数据
     // events  数组元素的总个数
     // maxevents 一次取出多少个数据
     //  timeout  -1:阻塞 0：不阻塞  ： >0 超时时间

     // 返回值 > 0返回就绪的IO数量和循环上线   0没有fd满足监听事件了  < 0  错误


     epoll 事件有两种模型 :
         //ET (Edge Triggered ) 边沿触发是有有数据来就触发,不管缓冲区是否有数据
         //LT (LEvel Triggered)水平触发 只要有数据 才会触发

    
    4.epoll ET模式 + 非阻塞 + void *ptr
      
      原来:socket bind listen -- epoll_create 创建监听红黑树 --- 返回 epfd 
          epoll_ctl 向树上添加fd  epoll_wait监听fd 的就绪事件  有事件满足条件  判断数组元素
          lfd满足是accpet 事件      cfd满足条件  read recv send write 等。。
    
    5.epoll 反应堆模型
         把对IO的管理转换成对事件的管理  不同的IO处理交给对应的处理回调函数 accept 的处理  数据可读
         数据可写  以及客户端的关闭连接