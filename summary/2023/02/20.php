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
?>

//方案2
<?php
 
db();
global $con;
 
//step1 接收下单参数
$product_id = 1;// 商品ID
$buy_num = 1;// 购买数量
 
//step2 下单前判断redis队列库存量
$redis = new \Redis();
$result = $redis->connect('127.0.0.1',6379);
$count = $redis->lpop('goods_store_' . $product_id);
if (!$count) {
    insertLog('error:no store redis');
    return '秒杀结束，没有商品库存了';
}
 
sleep(1);
//step3 更新商品库存数量（减去下单数量）
$sql = "update products set store=store-{$buy_num} where id={$product_id}";
if (mysqli_query($con, $sql)) {
    echo "更新成功";
    //step4 生成订单号创建订单
    $oid = build_order_no();
    create_order($oid, $product_id, $buy_num);
    insertLog('库存减少成功，下单成功');
} else {
    echo "更新失败";
    insertLog('库存减少失败');
}
 
function db()
{
    global $con;
    $con = new mysqli('localhost','root','root','test');
    if (!$con) {
        echo "数据库连接失败";
    }
}
 
/**
 * 生成唯一订单号
 */
function build_order_no()
{
    return date('Ymd') . str_pad(mt_rand(1, 99999), 5, '0', STR_PAD_LEFT);
}
 
function create_order($oid, $product_id, $number)
{
    global $con;
    $sql = "INSERT INTO `order` (oid, product_id, number) values('$oid', '$product_id', '$number')";
    mysqli_query($con, $sql);
}
 
/**
 * 记录日志
 */
function insertLog($content)
{
    global $con;
    $sql = "INSERT INTO `order_log` (content) values('$content')";
    mysqli_query($con, $sql);
}

// .redis 乐观锁防止超卖

$redis =new Redis();
$redis->connect("127.0.0.1", 6379);
$redis->watch('sales');//乐观锁 监视作用 set()  初始值0
$sales = $redis->get('sales');
 
$n = 20;// 库存
if ($sales >= $n) {
    exit('秒杀结束');
}
 
//redis开启事务
$redis->multi();
$redis->incr('sales'); //将 key 中储存的数字值增一 ,如果 key 不存在，那么 key 的值会先被初始化为 0 ，然后再执行 INCR 操作。
$res = $redis->exec(); //成功1 失败0
 
if ($res) {
    //秒杀成功
    $con = new mysqli('localhost','root','root','test');
    if (!$con) {
        echo "数据库连接失败";
    }
 
    $product_id = 1;// 商品ID
    $buy_num = 1;// 购买数量
    sleep(1);
 
    $sql = "update products set store=store-{$buy_num} where id={$product_id}";
 
    if (mysqli_query($con, $sql)) {
        echo "秒杀完成";
    }
 
} else {
    exit('抢购失败');
}
//https://www.cnblogs.com/ianlab/p/16357749.html