<?php

include dirname(__FILE__).'/client.php';

$params= ['name','zpw','user'];  //接口数据

$msg = json_encode($params);

$client = new Client();
$i = 0;
while(true)
{   
    if($i > 10000){
        break;
    }
    $client->send($msg);
    $i++;
}

echo "[".date("Y-m-d H:i:s")."]执行完成".PHP_EOL;