<?php
$server = new Swoole\Server('127.0.0.1', 9503);

$server->on('start', function ($server) {
    echo "TCP Server is started at tcp://127.0.0.1:9503\n";
});

$server->on('connect', function ($server, $fd){
    echo "connection open: {$fd}\n";
});

$server->on('receive', function ($server, $fd, $reactor_id, $data) {
    $server->send($fd, "Swoole: {$data}");
});

$server->on('close', function ($server, $fd) {
    echo "connection close: {$fd}\n";
});
echo swoole_version();
$server->start();