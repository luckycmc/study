<?php
/**
 * Created by PhpStorm.
 * User: zpw
 * Date: 2023/5/14
 * Time: 16:11
 */

namespace app\common\model;


use think\facade\Cache;
use think\facade\Db;
use think\Model;

class Goods extends Model
{
    /**
     * 对应的数据表名
     * @var string
     */
     protected $table = 'lmrs_products';

    /**
     * 获取数据排名
     * @return mixed
     */
     public function getRank()
     {
         //先从redis获取 没有则在从 数据库中获取
         $data = Cache::get('g_rank');
         if (!$data){
             $data = Db::name('lmrs_products')
                 ->field('name,sold_count')
                 ->order('sold_count desc')
                 ->limit(10)
                 ->select();
             Cache::set('g_rank',$data,60);
         }
         return $data;
     }

}// class end