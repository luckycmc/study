<?php
/**
 * 这个主要讲解 request 的案例
 */
//创建http server
$http = new Swoole\Http\Server('0.0.0.0', 9501);
//设置1个进程
$http->set([
    //'worker_num' => Swoole_cpu_num(),
    'worker_num' => 2,
]);
$http->on('start', function ($server) {
    echo "Swoole http server is started at http://127.0.0.1:9501\n";
});
//所有的http 请求都在这里面 我们开发的框架的 文件也都会 加载到这个接口里面
// 每一个请求一个携程
$http->on('request', function(Swoole\Http\Request $request, Swoole\Http\Response $response){
    //打印header
    //print_r($request->header);
    //echo $request->header['host'].PHP_EOL;        //输出header host
    //echo $request->header['user-agent'].PHP_EOL;  // user-agent
    //查看呢当前进程和进程容器的协程id
    echo 'pid------'.posix_getpid().'------- cid:'.\Swoole\Coroutine::getCid().PHP_EOL;
    $response->header('Content-Type', 'text/plain');
    $response->end('Hello World request');
});
//开启http 服务
$http->start();
// 
//其他的属性小伙伴们可以按照上述的方法去调试

