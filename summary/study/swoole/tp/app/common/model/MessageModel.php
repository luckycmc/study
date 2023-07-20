<?php
/**
 * Created by PhpStorm.
 * User: zpw
 * Date: 2023/7/20
 * Time: 19:11
 */

namespace app\common\model;


use think\Model;

class MessageModel extends Model
{
    /**
     * 操作对应的数据表
     * @var string
     */
    protected $table = 'test_message';
}