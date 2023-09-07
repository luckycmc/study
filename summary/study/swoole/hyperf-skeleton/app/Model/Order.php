<?php
/**
 * Created by PhpStorm.
 * User: zpw
 * Date: 2023/9/7
 * Time: 17:06
 */
declare (strict_types=1);
namespace App\Model;


class Order extends Model
{
    /**
     * 订单表
     * @var string
     */
    protected $table = "ku_order";

    protected  $connection = 'goods';
}