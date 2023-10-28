<?php

$http = new Swoole\Http\Server('0.0.0.0', 9501);

$http->on('start', function ($server) {
    echo "Swoole http server is started at http://127.0.0.1:9501\n";
});
$request = new Request();
//绑定类中的函数  注意 绑定的函数必须是 public 属性 否则会用权限的问题
$http->on('request', [$request,'request']);
//banding 函数的处理
//$http->on('request','request');
$http->start();
//banding 函数的处理
/*function request($request, $response) {
    $response->header('Content-Type', 'text/plain');
    $response->end('Hello World request');
}*/
class Request{
    public function request($request, $response)
    {
        $response->header('Content-Type', 'text/plain');
        $response->end('Hello World request class');
    }
}