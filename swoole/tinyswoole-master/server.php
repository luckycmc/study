<?php
$port = 9501;

function onStart()
{    
    global $port;
    print_r("Server IP is 127.0.0.1 port is $port:" . PHP_EOL);
    print_r("server is running..." . PHP_EOL);
}

function onConnect($fd)
{
    print_r("client[{$fd}] is connected" . PHP_EOL);
}

function onReceive($serv, $fd, $data)
{
    print_r("receive data from client[{$fd}]: {$data}");
    // 发送给客户端的数据
    $serv->send($fd, "hello client");
}

$serv = new TinySwoole\Server('127.0.0.1', $port, TSWOOLE_TCP);

$serv->set([
    'reactor_num' => 2,
    'worker_num' => 4,
]);

$serv->on("Start", "onStart");
$serv->on("Connect", "onConnect");
$serv->on("Receive", "onReceive");
$serv->start();
