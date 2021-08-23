/**
      在没有epoll 的前提下
      1.poll/select
      2.多进程和多线程  以及多协成
      select 的问题
        1.数量的问题
        2.拷贝的问题
        3.内存的问题

      1.数量 是所有IO集合的数量
         所有的IO放在一起 数据结构
      2.少量的io就绪，结构体
      一次次添加到整个集合
      3.数据结构  用什么样数据结构存贮
      
      服务器底成原理
      while(1){
          int ready = epoll_wait();//检测io
          for(i =0;i<ready;i++){
              
               recv(buffer); //操作IO
                 parse();  //解析数据

                send();  //所有的地城服务器操作
          }
      }

*/