<?php

/**
 * Class Test
 * 先打开浏览器输入：http://127.0.0.1:9501/?key=sleep  (我们期望输出sleep)
再输入：http://127.0.0.1:9501/?key=abcd
最终第一个输出的是abcd, 这是因为，在第一个请求处理过程中，第二个请求的时候，
 * 已以把Test::$key改变了，当第一个请求处理完，读到的数据已经变了
 2.3、什么情况下可以用全局变量

我认为满足下面两个条件，可以使用全局变量，否则避要完全避免掉全局变量

1.有跨请求的需要

2.只读
 */
class Test
{
    static $key = [];
}

$http = new Swoole\Http\Server("0.0.0.0", 9501);
$http->set([
    //"daemonize" => true,
    "worker_num" => 1,
]);
$http->on('request', function ($request, $response) {
    if ($request->server['path_info'] == '/favicon.ico') {
        $response->end('');
        return;
    }
    $key = $request->get['key'];
    Test::$key = $key;
    if ($key == 'sleep') {
        //模拟耗时操作
        Co::sleep(10);
    }
    $response->end(Test::$key);
});
$http->start();