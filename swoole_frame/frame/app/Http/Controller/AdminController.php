<?php
/**
 * Created by PhpStorm.
 * User: f
 * Date: 2020/11/4
 * Time: 17:55
 */

namespace App\Http\Controller;

use sethink\swooleOrm\Db;


class AdminController
{
    //查询数据库
    public function get()
    {

        $rand_id = rand(10,10000);
        //var_dump(ManagerDB::getInstance()->get());
        $result = Db::init(\SwoStar\Model\Db::getInstance()->get())
                  ->query('select * from tp_user ORDER BY id desc limit '.$rand_id.', 10');

        return json_encode($result);
    }

}// class end