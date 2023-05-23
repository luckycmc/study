<?php
$ip = '0.0.0.0'; $port = 9501;  //端口和对应的ip
//创建http 服务器
$http = new Swoole\Http\Server($ip,$port);
$_array = [];
//设置事件  我们这里添加一下依赖注入 这是一个回调函数
$http->on('request',function (\Swoole\Http\Request $request,\Swoole\Http\Response $response) use($http){
        // $request 主要回去 请求响应的信息 var_dump($request);
        //var_dump($request);
        //$cid = Swoole\Coroutine::getCid();  //获取当前协成的id 我们可以看到 一个请求对应一个协成
        //var_dump($cid);
        //$response 响应对象
        //$response->end("<h1>Hello WWW.PHP.CN</h1>");
    global $_array;  //PHP的全局变量swoole无法销毁
    //请求 /a（协程 1 ）
    if ($request->server['request_uri'] == '/a') {
        $_array['name'] = 'a';
        co::sleep(10.0);
        var_dump($_array);
        $response->end($_array['name']);
        unset($_array['name']); //只能手动销毁 否则会产生数据污染
    }
    //请求 /b（协程 2 ）
    elseif($request->server['request_uri'] == '/b') {
        $_array['name'] = 'b';
        $response->end();
    }else{
        var_dump($_array);
    }
});

//启动服务器
$http->start();