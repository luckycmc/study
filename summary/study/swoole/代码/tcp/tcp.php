<?php
//创建Server对象，监听 127.0.0.1:9501 端口
$server = new Swoole\Server('127.0.0.1', 9506);

//监听连接进入事件 当客户端有连接的时候会触发这个回调函数
$server->on('Connect', function (\Swoole\Server $server, $fd) {
    //终端会输出对应的文件描述符
    echo "{$fd}Client: Connect.\n";
});

//监听数据接收事件 接受客户端发送的数据
/**
 * 参数说明 $server 是服务的对象 $fd 对应的文件描述符
 * $reactor_id 表示接受数据的 reactor 线程
 * $data 用户发送的数据
 */
$server->on('Receive', function (\Swoole\Server $server, $fd, $reactor_id, $data) {
    echo "reactor_id is {$reactor_id}".PHP_EOL;
    $server->send($fd, "Server: {$data}".date('Y-m-d H:i:s',time()));
});

//监听连接关闭事件 当前客户端关闭时会触发这个回调函数
$server->on('Close', function (\Swoole\Server $server, $fd) {

    echo "{$fd}Client: Close.\n";
});

//启动服务器
$server->start();
