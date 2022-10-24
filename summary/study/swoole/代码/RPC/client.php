<?php
//创建TCP客户端   相当于C创建socket
$client = new \Swoole\Client(SWOOLE_SOCK_TCP);

$client->connect('127.0.0.1',9501);  //完成三次握手
//调用的控制器和方法
$data = json_encode(['site'=>'Goods','action'=>'index']);
$client->send($data);  // 发送数据
//接受服务端的消息
$result = $client->recv(); // 接受数据
$client->close();  // 客户端关闭
print_r($result);