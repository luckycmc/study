<?php
declare(strict_types=1);

namespace App\Controller;

use App\Common\OrderInfo;
use App\Model\Goods;
use Hyperf\DbConnection\Db;
use Hyperf\HttpServer\Contract\RequestInterface;
use Hyperf\HttpServer\Annotation\AutoController;

#[AutoController]
class UserController
{
    // Hyperf 会自动为此方法生成一个 /user/index 的路由，允许通过 GET 或 POST 方式请求
    public function user(RequestInterface $request)
    {
        // 从请求中获得 id 参数
        $id = $request->input('id', 1);
        return (string)$id;
    }
    public function goods(RequestInterface $request)
    {
        $user = $request->input('list', 'Hyperf');
        $method = $request->getMethod();
        $data = Db::connection('goods')->select('select * from ku_goods');
        $user_id = rand(1,100);
        return [
            'method' => $method,
            'message' => "Hello {$user}.",
            'order_num'=>OrderInfo::getOrderNo($user_id),//加一个用户id
            'data'=>json_encode($data),
        ];
    }

    /**
     * 订单v1版本
     */
    public function v1(RequestInterface $request)
    {
        $user_id = rand(1,100);
        if ($user_id ==0){
            return "params is error";
        }
        //查询商品
        $goods = Goods::find(1);
        if (!$goods){
            return "goods is error";
        }
        //2.判断库存
        if ($goods->num < 1){
            return "goods num is zero";
        }
        //3.减少库存
        $flag = Db::update("update ku_goods set num = num - 1 where id = {$goods->id} AND num > 0");
        if (!$flag) {
            return "goods num is not num";
        }
        $order_num = OrderInfo::getOrderNo();
        return $order_num;
        //4.添加订单信息
        $order = new \App\Model\Order();
        $order->user_id = $user_id;
        $order->goods_id = $goods->id;
        $order->created_at = date("Y-m-d H:i:s",time());
        $order->updated_at = date("Y-m-d H:i:s",time());
        $result = $order->save();
        return $result?"success":"faild";
    }
}// class end
