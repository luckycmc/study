<?php
/**
 * Created by PhpStorm.
 * User: f
 * Date: 2020/10/10
 * Time: 14:32
 */

namespace App\Http\Controller;
use sethink\swooleOrm\Db;
use sethink\swooleRedis\CoRedis;
use SwoStar\Model\Re;

/**
 * 首页控制器
 * Class IndexController
 * @package App\Http\Controller\
 */
class IndexController
{

    //测试方法
    public function dd()
    {
        $id = rand(1,100);
        $result = Db::init(\SwoStar\Model\Db::getInstance()->get())
                   ->name('user')
                   ->insert(['title'=>'zpw'.$id]);
       if ($result){
           return "数据插入成功!";
       }else{
           return "error";
       }
    }

    //查询数据库
    public function list()
    {

        $rand_id = rand(10,10000);
        //var_dump(ManagerDB::getInstance()->get());
        $result = Db::init(\SwoStar\Model\Db::getInstance()->get())
                  ->query('select * from tp_user ORDER BY id desc limit '.$rand_id.', 10');

        return json_encode($result);

    }
    //获取redis 信息
    public function get()
    {

        $rs2 = CoRedis::init(Re::getInstance()->get())->get('zpw');
        return json_encode($rs2);
    }

} // class end