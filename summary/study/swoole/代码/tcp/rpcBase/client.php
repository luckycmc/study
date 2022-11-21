<?php
//客户端：
$cli = new \Swoole\Client(SWOOLE_SOCK_TCP);
$cli->connect('127.0.0.1', 8888);
$data = [
    'class'=>'goods',
    'method'=>'info',
    'param'=>[
        'uid' => 1,
        'type'=>2,
    ],
];
$data = json_encode($data);
$cli->send($data);
$result=$cli->recv();//接收消息
$cli->close();
var_dump($result);