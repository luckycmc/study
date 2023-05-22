<?php
/**
 * Created by PhpStorm.
 * User: f
 * Date: 2023/5/16
 * Time: 10:27
 */

namespace app\controller;


use app\BaseController;
use think\facade\Db;

/**
 * 商品模块
 * Class Goods
 * @package app\controller
 */
class Goods extends BaseController
{
    /**
     * 获取商品的所有信息
     * @return string
     */
    public function goodsList()
    {
        $data = Db::name('lmrs_products')
                ->order('sold_count desc')
                ->limit(10)
                ->select();
        return json_encode($data);
    }
    /**
     * 初始化
     * @return int
     */
    public function init()
    {
        $num = 100;
        $gid = 1;   // 商品编号
        $redis = getRedis();
        for($i = 0; $i < $num; $i++) {
            $redis->rpush('seckill_goods:'.$gid, 1);  // 将商品push到商品列表, 值无意义
        }
        $len = $redis->llen('seckill_goods:'.$gid);
        return $len;
    }

    /**
     * 商品秒杀
     */
    public function skill()
    {
          $uid = rand(1,100);
          $gid = 1;//商品编号
          $redis = getRedis();
          //首先判断是否 参加过 没人只参加一次
          $result = $redis->sIsMember('seckill_user:'.$gid,$uid);
          if ($result){
             return '你已经参加过抢购了';
          }
          //秒杀商品
          $len = $redis->lPop('seckill_goods:'.$gid); //列表为空则强完毕
          if (!$len){
              return '已经请购完毕';
          }
          //抢购成功的如redis集合
         $redis->rPush('seckill_order:'.$gid,$uid);
         //将该用也加入到集合
         $redis->sAdd('seckill_user:'.$gid,$uid);
         return '恭喜，抢购成功';
    }
    /**
     * 商品秒杀
     */
    public function skillWatch()
    {
        $uid = rand(1,10);
        $gid = 1;//商品编号
        $redis = getRedis();
        //首先判断是否 参加过 没人只参加一次
        $redis->watch('seckill_goods_mult:'.$gid);
        $result = $redis->sIsMember('seckill_user:'.$gid,$uid);
        if ($result){
            return '你已经参加过抢购了';
        }
        //秒杀商品
        $len = $redis->get('seckill_goods_mult:'.$gid); //列表为空则强完毕
        if (!$len){
            return '已经请购完毕';
        }
        $redis->multi();//开启事务
            //抢购成功的如redis集合
            $redis->decr('seckill_goods_mult:'.$gid);
            //将该用也加入到集合
            $redis->sAdd('seckill_user:'.$gid,$uid);
        $result = $redis->exec();//执行事务
        //这里执行成功可以指向mysql下单业务
        if ($result){
            //这里是抢购成功的业务
            //例如：更新商品库存
            $data = Db::name('lmrs_products')->where(['id'=>1])->inc('sold_count')->update();
            var_dump($data);
        }
        return '恭喜，抢购成功';
    }

    /**
     * mysql事务
     */
    public function xsCommit()
    {
        Db::startTrans();
        try{
            $goods_id = rand(1,251616);
            $data = Db::name('lmrs_products')->where(['id'=>$goods_id])->inc('sold_count')->update();
            $data = Db::name('lmrs_user_login_log')->where(['id'=>3])->inc('login_type')->update();
            //提交事务
            Db::commit();
        }catch (\Exception $e){
            var_dump($e->getMessage());
            //事务回滚
            Db::rollback();
        }
    }

    /**
     * 更新数据测试
     */
    public function updateGoods()
    {
        $goods_id = rand(1,25166);
         Db::name('lmrs_products')->where(['id'=>30])->inc('sold_count')->update();
    }

    /**
     *  销量排行榜
     * @return string
     */
    public function rank()
    {
        $data = \app\common\model\Goods::create([])->getRank();
        return json_encode($data);
    }

    //用户取模 雪花算法
    
}// class end