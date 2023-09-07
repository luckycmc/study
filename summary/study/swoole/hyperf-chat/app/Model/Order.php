<?php
/**
 * Created by PhpStorm.
 * User: zpw
 * Date: 2023/9/3
 * Time: 19:20
 */

namespace App\Model;


class Order extends Model
{
    /**
     * 142服务器库的
 * 订单表
 * @var string
 */
    protected $table = "ku_order";
    /**
     * 对应的连接库
     * @var string
     */
    protected $connection ="order";
}