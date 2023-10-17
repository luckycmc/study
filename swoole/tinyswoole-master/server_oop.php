<?php

class server_oop{
     
      private $server;
      private $port = 9502;
      public function __construct()
      {
        $this->server = new TinySwoole\Server('127.0.0.1', $this->port, TSWOOLE_TCP);
        //设置配置参数
        $this->server->set([
            'reactor_num' => 2,
            'worker_num' => 4,
        ]);
        //绑定一个地址空间 给用户执行
        $this->server->on("Start",  [$this, "onStart"]);
         $this->server->on("Connect", [$this,"onConnect"]);
         $this->server->on("Receive", [$this,"onReceive"]);
         $this->server->start();
      }  
      
       public function onStart()
        {    
           
            print_r("Server IP is 127.0.0.1 port is $this->port:" . PHP_EOL);
            print_r("server is running..." . PHP_EOL);
        }

        public function onConnect($fd)
        {
            print_r("client[{$fd}] is connected" . PHP_EOL);
        }

        public function onReceive($serv, $fd, $data)
        {
            print_r("receive data from client[{$fd}]: {$data}". PHP_EOL);
            // 发送给客户端的数据
            $serv->send($fd, "hello client");
        }
}

new server_oop();