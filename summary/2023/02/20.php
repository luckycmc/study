<?php
//通过redis乐观锁的和redis队列两种方式来实现秒杀时的下单逻辑
$redis = new \Redis();
$redis->connect('127.0.0.1',6379);
$watchKey = 'skill_goods';
//监控要执行的key
$redis->watch($watchKey);
$redis->multi(); //开启事务
//获取剩余的数量
$result = $redis->get($watchKey);
$redis->decr($watchKey); //减少库存
//增加数据到对应的集合中收集用户的抢购信息
$redis->exec(); //提交事务
$result = $redis->get($watchKey);
if (($result)){
   echo "faild".PHP_EOL;
}else{
    echo "当前数量还剩".$result.PHP_EOL;
}
