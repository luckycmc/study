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
    //这个是我本机的绝对路径
    'document_root' => '/data/C_study/example/base/http', // v4.4.0以下版本, 此处必须为绝对路径
    'enable_static_handler' => true,
]);
$http->on('start', function ($server) {
    //
    echo "Swoole http server is started at http://127.0.0.1:9501\n";
});

$http->on('request', function(Swoole\Http\Request $request, Swoole\Http\Response $response){
    
    //static $a[] = 'a'; //切记静态数组 不释放可能会让数组无线扩大
    //var_dump($a);
    //设置 header 头 输出内容为文本
    $response->header('Content-Type', 'text/plain');
    //输入内容为 jpeg
    //$response->header('content-type', 'image/jpeg', true);
    //设置json的方式
    //输出数据给客户端 也就是浏览器
    $response->end('index document_root');
});
//开启http 服务
$http->start();
// 
//其他的属性小伙伴们可以按照上述的方法去调试


