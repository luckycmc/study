<?php
 $http = new Swoole\Http\Server('0.0.0.0', 9501);

 $http->set([
     'worker_num'=> swoole_cpu_num() * 2,
     //'worker_num'=> 1,
 ]);
 //进程服务启动的时候
 $http->on('start', function ($server) {
     echo "Swoole http server is started at http://127.0.0.1:9501\n";
 });

 $http->on('request', function ($request, $response) {
     $response->header('Content-Type', 'text/plain');
     $response->end('Hello World\r\n');
 });
 $http->start();

 // ls -al /proc/20945/fd  查看某个进程下对应的文件描述符
 //  ls -al /proc/20945/net/unix  查看某个进程下对应的unix socket
 // netstat -antp | grep pid | wc -l
 // ls -al | grep socket | wc -l
 // 1.主进程的中的master 负责accept 连接之后  会将这个链接分配给固定的reactor 线程 并由这个线程负责监听此socket
 // 2.在socket可读时读取数据，并进行协议解析，将请求投递到Worker进程
 // 3.在socket可写时将数据发送给TCP客户端。
// listen fd 和accept fd 不一样的 

// 一下 listen() 函数的第二个参数（ backlog）的作用：告诉内核连接队列的长度。

// 在accept 是在 客户端与服务器端已经建立好了连接 

//accept()函数
//accept()函数功能是，从处于 established 状态的连接队列头部取出一个已经完成的连接，
//如果这个队列没有已经完成的连接，accept()函数就会阻塞，直到取出队列中已完成的用户连接为止。
// 会产生一个新的fd 也就是与客户端端通讯的fd
// http://rango.swoole.com/archives/305 reactor 和worker的关系
/* 
   master 线程 reactor 线程与worker  进程之间的通讯
   master进程中的主线程负责监听端口(listen), 接受连接(accept, 产生一个fd), 接受连接后将请求分配给reactor线程, 
   默认通过fd % reactor_num进行分配, 之后通过epoll_ctl将fd加入到对应reactor线程中(如果对应的reactor线程正在执行epoll_wait, 主线程会阻塞), 
   刚加入时监听写事件, 如果直接监听读事件, 可能会立刻被触发, 而监听写事件可以允许reactor线程进行一些初始化操作

   eactor线程中fd的写事件被触发, reactor线程负责处理, 发现是首次加入, 没有数据可写, 则开启读事件监听
   reactor线程读取到用户的请求数据, 一个请求的数据接收完后, 将数据转发给worker进程, 默认是通过fd % worker_num进行分配
   reactor发送给worker进程的数据包, 会包含一个头部, 头部中记录了reactor的信息
  如果发送的数据过大, 则需要将数据进行分片, 限于篇幅, reactor的分片, 后续再进行详细讲述
   可能存在多个reactor线程同时向同一个worker进程发送数据的情况, 故而swoole采用SOCK_DGRAM模式与worker进程进行通信, 通过每个数据包的包头, worker进程可以区分出是由哪个reactor线程发送的数据
   worker进程收到reactor发送的数据包后, 进行处理, 处理完成后, 将数据发送给主进程
   worker进程发送给主进程的数据包, 也会包含一个头部, 当reactor线程收到数据包后, 能够知道对应的reactor线程, 请求的fd等信息
  主进程收到worker进程发送的数据包, 这个会触发某个reactor线程进行处理
  这个reactor线程并不一定是之前发送请求给worker进程的那个reactor线程
  主进程的每个reactor线程都负责监听worker进程发送的数据包, 每个worker发送的数据包只会由一个reactor线程进行监听, 故而只会触发一个reactor线程
   reactor线程处理worker进程发送的数据包, 如果是直接发送数据给客户端, 则可以直接发送, 如果需要改变这个这个连接的监听状态(例如close), 则需要先找到监听这个连接的reactor, 进而改变这个连接的监听状态
   reactor处理线程与reactor监听线程可能并不是同一个线程
  reactor监听线程负责监听客户端发送的数据, 进而转发给worker进程
   reactor处理线程负责监听worker进程发送给主进程的数据, 进而将数据发送给客户端
*/