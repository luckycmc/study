<?php
/**
 * Created by PhpStorm.
 * User: f
 * Date: 2020/10/26
 * Time: 13:12
 */

namespace SwoStar\Model;


use sethink\swooleRedis\RedisPool;
use SwoStar\Core\Singleton;

class ManagerRedis
{
   use Singleton;

    /**
     * redis 连接池
     */
    public function init()
    {
        $redisConfig = app('config')->get('redis');
        $redisPool   =  new RedisPool($redisConfig);
        unset($redisConfig);
        //启动定时器定期清除对应的连接
        $redisPool->clearTimer();
        return $redisPool;
    }

} // class end