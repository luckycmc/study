<?php
$http_server = new Swoole\Http\Server('0.0.0.0',9998);
//设置配置信息
$http_server->set([
    'daemonize'=> false
]);
$http_server->on('request', function (\Swoole\Http\Request $request, \Swoole\Http\Response $response) {
    $response->header("Content-Type", "text/html; charset=utf-8");
    $response->end("<h1>Hello Swoole. #".rand(1000, 9999)."</h1>");
});

//多监听一个TCP端口，对外开启TCP服务，并设置TCP服务器的回调
$tcp_server = $http_server->listen('0.0.0.0', 9999, SWOOLE_SOCK_TCP);
//默认新监听的端口 9999 会继承主服务器的设置，也是 HTTP 协议
//需要调用 set 方法覆盖主服务器的设置
$tcp_server->set([]);
$tcp_server->on('receive', function (\Swoole\Server $server, int $fd, int $threadId, string $data) {
    echo $data.PHP_EOL;
    $server->send($fd,$data); //数据发送给客户端
});

$http_server->start();