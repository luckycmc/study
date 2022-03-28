<?php

function onStart()
{
    print_r("server is running..." . PHP_EOL);
}

function onConnect($fd)
{
    print_r("client[{$fd}] is connected" . PHP_EOL);
}

function onReceive($serv, $fd, $data)
{
    print_r("receive data from client[{$fd}]: {$data}");
    $serv->send($fd, "hello client\n\r");
}

function onClose($fd)
{
    print_r("client[{$fd}] is not connected" . PHP_EOL);
}
$serv = new TinySwoole\Server('127.0.0.1', 9501, TSWOOLE_TCP);

$serv->set([
    'reactor_num' => 2,
    'worker_num' => 4,
]);

$serv->on("Start", "onStart");
$serv->on("Connect", "onConnect");
$serv->on("Receive", "onReceive");
$serv->on("Close", "onClose");
//启动服务器
$serv->start();
