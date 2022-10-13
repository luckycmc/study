<?php
/*****
 * 服务器的设置 和启动
 */
class Server
{    
     protected $server;
     protected $IP = 9501;
     protected $port = '0.0.00';

     public function __construct()
     {     
           //创建PHP 服务器
           $this->server = new Swoole\Http\Server($this->ip,$this->port);
           // 设置php服务器属性

     }
    
     public function startServer()
     {

     }

}// cklass end