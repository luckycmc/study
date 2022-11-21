<?php
$server = new \Swoole\Server('127.0.0.1',8888);
$server->set([
     'worker_num'=>2,
]);
//客户端接受数据
$server->on('receive',function (\Swoole\Server $server,$fd,$reactor_id,$data){

      //$data 是接受客户端的数据
      $data = json_decode($data,true);
      //更具客户端发送过来的数据进行对应的操作 例如 控制器 方法 参数
      print_r($data);
       //伪代码
      //(new $data['class'])->$data['method']($data['param'])
      //数据发送给客户端
      $server->send($fd,json_encode(['msg'=>'ok']));
});

//服务器启动
$server->start();