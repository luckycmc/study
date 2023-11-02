<?php
/**
 * 这个主要讲解 response 的案例
 */
//创建http server
$http = new Swoole\Http\Server('0.0.0.0', 9501);
//设置1个进程
$http->set([
    //'worker_num' => Swoole_cpu_num(),
    'worker_num' => 1,
]);
$http->on('start', function ($server) {
    echo "Swoole http server is started at http://127.0.0.1:9501\n";
});

$http->on('request', function(Swoole\Http\Request $request, Swoole\Http\Response $response){
    
    static $a[] = 'a'; //切记静态数组 不释放可能会让数组无线扩大
 
    var_dump($a);
    $response->header('Content-Type', 'text/plain');
    $response->end('Hello World request');
});
//开启http 服务
$http->start();
// 
//其他的属性小伙伴们可以按照上述的方法去调试


