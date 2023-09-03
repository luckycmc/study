<?php
namespace App\Controller\Goods;
use App\Controller\AbstractController;
use App\Model\Goods;
use app\Request;
use Hyperf\DbConnection\Db;
use Hyperf\HttpServer\Annotation\Controller;
use Hyperf\HttpServer\Annotation\RequestMapping;

/**
 * 订单控制器
 * @Controller(prefix="/goods/order")
 */
class Order extends AbstractController
{
    /**
     * v1 版本下单
     * 出现此问题的原因就在 判断是否有库存 和 减库存之间，
     * 多个线程过来获取到还有剩余库存，然后更新库存，
     * 但是在更新库存之前，其它线程已经抢完了，造成超卖
     * @RequestMapping(path="v1",methods="get")
     */
    public function v1()
    {
        $user_id = $this->request->input('user_id') + 0;
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
        $flag = Db::update("update ku_goods set num = num - 1 where id = {$goods->id}");
        if (!$flag) {
            return "goods num is not num";
        }
        //4.添加订单信息
        $order = new \App\Model\Order();
        $order->user_id = $user_id;
        $order->goods_id = $goods->id;
        $order->created_at = date("Y-m-d H:i:s",time());
        $order->updated_at = date("Y-m-d H:i:s",time());
        $result = $order->save();
        return $result?"success":"faild";
    }
    /**
     * 乐观锁
     可以看到，商品未出现超卖，但是有另外的问题，100 个商品竟然都没有强光
     此时的原因是 获取商品和更新库存之间，其它线程已经更改了库存，
     * 导致此线程更新失败
     * @RequestMapping(path="v2",methods="get")
     * @return string
     */
     public function v2()
     {
         $user_id = $this->request->input('user_id') + 0;
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
         $flag = Db::update("update ku_goods set num = num - 1 where id = {$goods->id} AND num = {$goods->num}");
         if (!$flag) {
             return "goods num is not num";
         }
         //4.添加订单信息
         $order = new \App\Model\Order();
         $order->user_id = $user_id;
         $order->goods_id = $goods->id;
         $order->created_at = date("Y-m-d H:i:s",time());
         $order->updated_at = date("Y-m-d H:i:s",time());
         $result = $order->save();
         return $result?"success":"faild";
     }
    /**
     * 乐观锁
     * @RequestMapping(path="v3",methods="get")
     * @return string
     */
    public function v3()
    {
        $user_id = $this->request->input('user_id') + 0;
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
        //4.添加订单信息
        $order = new \App\Model\Order();
        $order->user_id = $user_id;
        $order->goods_id = $goods->id;
        $order->created_at = date("Y-m-d H:i:s",time());
        $order->updated_at = date("Y-m-d H:i:s",time());
        $result = $order->save();
        return $result?"success":"faild";
    }

    /**
     * 判断用户是否购买过
     */
    public function v4()
    {
         //查订单表
         // redis 获取对应的用户是否购买
    }
    
}// class end