<?php
//创建tcp 服务器
// 相当于完成 bind socket accept
$server = new \Swoole\Server('0.0.0.0',9501);

$server->set([
     'worker_num'=>swoole_cpu_num(),
]);
//注册进程启动事件
$server->on('WorkerStart','onWorkerStart');
//进程启动
function onWorkerStart(\Swoole\Server $server,int $worker_id)
{
     // 注册自动加载
     spl_autoload_register('myAutoLoad',true,true);
}
//自动加载函数
function myAutoLoad($className)
{
    $file = './service/'.$className.'.php';
    if (file_exists($file)){
        require $file;
    }else{
        echo $file.' file is not found '.PHP_EOL;
    }

}
//接受数据的事件  接受数据相当于 recv 
$server->on('receive',function (\Swoole\Server $ser,$fd,$from_id,$data){

        //接受客户端的数据 本质上也是客户端和tcp_server的请求
        $data = json_decode($data,true);  //定义好相应的数据格式 进行数据通讯
        // 获取对应的service 例如 Service/Goods
       //加载对应的服务
        $action     = $data['action'];
        $controller = $data['site'];
        //获取对应的服务信息
        $reurnData = (new $controller())->$action();
        //返回数据给客户端 相当于write
        $ser->send($fd,json_encode($reurnData));
});
//启动服务器
$server->start();