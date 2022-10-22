<?php
//创建TCP客户端
$client = new \Swoole\Client(SWOOLE_SOCK_TCP);

$client->connect('127.0.0.1',9501);
//调用的控制器和方法
$data = json_encode(['site'=>'user','action'=>'index']);
$client->send($data);
//接受服务端的消息
$result = $client->recv();
$client->close();
print_r($result);