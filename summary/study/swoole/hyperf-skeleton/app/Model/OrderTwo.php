<?php
declare(strict_types=1);
/**
 * Created by PhpStorm.
 * User: zpw
 * Date: 2023/9/9
 * Time: 17:01
 */

namespace App\Model;


class OrderTwo extends Model
{
    protected ?string $table = 'ku_order';
    /**
     * 对应的连接器
     */
    protected ?string $connection = "goods";
}