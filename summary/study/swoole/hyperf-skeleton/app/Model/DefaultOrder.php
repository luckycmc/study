<?php
/**
 * Created by PhpStorm.
 * User: zpw
 * Date: 2023/9/7
 * Time: 19:38
 */
declare (strict_types=1);

namespace App\Model;

/**
 * 默认库的
 * Class DefaultOrder
 * @package App\Model
 */
class DefaultOrder extends Model
{
    /**
     * 订单表
     * @var string
     */
    protected $table = "ku_order";
    /**
     * 对应的链接库
     * @var string
     */
    protected $connection = "default";
}