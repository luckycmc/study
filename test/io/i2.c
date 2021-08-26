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
    //数据先考培到内核的缓存区，然后再从内核缓去拷贝到 用户空间
     网络IO的本质是socket的读取，socket在linux系统被抽象为流，IO可以理解为对流的操作。对于一次IO访问（以read举例），
     数据会先被拷贝到操作系统内核的缓冲区中，然后才会从操作系统内核的缓冲区拷贝到应用程序的地址空间。
     所以说，当一个recv操作发生时，它会经历两个阶段
     第一阶段是:等待数据拷贝（Waiting for the data to be ready）
     第二阶段是:将数据拷贝到用户空间的进程中（Copying the data from the kernel to the process）   
     对于socket流而言：
     第一步：通常涉及等待网络上的数据分组到达，然后被复制到内核的某个缓冲区。
     第二步：把数据从内核缓冲区复制到应用进程缓冲区。 
*/