<?php
$port = 9502;

function onStart()
{    
    echo "onStart php execute".PHP_EOL;
}

function onConnect($fd)
{
    print_r("client[{$fd}] is connected" . PHP_EOL);
}

function onReceive($serv, $fd, $data)
{
    print_r("receive data from client[{$fd}]: {$data}". PHP_EOL);
    // 发送给客户端的数据
    $serv->send($fd, "hello client");
}

$serv = new TinySwoole\Server('127.0.0.1', $port, TSWOOLE_TCP);
//设置配置项
$serv->set([
    'reactor_num' => 1,
    'worker_num' => 2,
]);
//传入函数地址空间即可 函数 类方法都可以 闭包函数
$serv->on("Start", function(){
    global $port;
    echo "Server IP is 127.0.0.1 port is $port:" . PHP_EOL;
    echo "server is running..." . PHP_EOL;
});
//注册函数的方式
$serv->on("Start", "onStart");
$serv->on("Connect", "onConnect");
$serv->on("Receive", "onReceive");
//启动 tinyswooleServer
$serv->start();
