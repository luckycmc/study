<?php
/**
 * Created by PhpStorm.
 * User: zpw
 * Date: 2020/10/25
 * Time: 18:06
 */

namespace SwoStar\Model;


use sethink\swooleOrm\MysqlPool;
use SwoStar\Core\Singleton;

class ManagerDB
{
    use Singleton;
    //数据库初始化
    public function init()
    {
          $databaeConfig = app('config')->get('databases');
          $MysqlPool = new MysqlPool($databaeConfig);

          return $MysqlPool;
    }

}// class end