<?php
//创建tcp 服务器
// 相当于完成 bind socket accept
$server = new \Swoole\Server('0.0.0.0',9501);

$server->set([
     'worker_num'=>swoole_cpu_num(),
]);
//接受数据的事件  接受数据相当于 recv 
$server->on('receive',function (\Swoole\Server $ser,$fd,$from_id,$data){

        //接受客户端的数据
        $data = json_decode($data,true);  //定义好相应的数据格式 进行数据通讯
        print_r($data);
        $returnData = [

               'name'=>'zpw',
               'age'=>'23',
               'sex'=>1,
        ];
        //返回数据给客户端 相当于write
        $ser->send($fd,json_encode($returnData));
});
//启动服务器
$server->start();