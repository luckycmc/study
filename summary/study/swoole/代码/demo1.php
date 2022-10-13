<?php
$ip = '0.0.0.0'; $port = 9501;  //端口和对应的ip
//创建http 服务器
$http = new Swoole\Http\Server($ip,$port);

//设置事件  我们这里添加一下依赖注入 这是一个回调函数
$http->on('request',function (\Swoole\Http\Request $request,\Swoole\Http\Response $response) use($http){
        // $request 主要回去 请求响应的信息 var_dump($request);
        //var_dump($request);
        //$cid = Swoole\Coroutine::getCid();  //获取当前协成的id 我们可以看到 一个请求对应一个协成
        //var_dump($cid);
        //$response 响应对象
        $response->end("<h1>Hello WWW.PHP.CN</h1>");
});

//启动服务器
$http->start();