<?php
/**
 * Created by PhpStorm.
 * User: zpw
 * Date: 2020/10/11
 * Time: 18:51
 */

namespace SwoStar\Server\WebSocket;


class Connections
{
    //记录用户连接
    /**
     * [
     *   fd => [
     *        'path ' =>xxx,
     *        'xxx'   => 000
     *    ]
     * ]
     */
    protected static $connections = [];
    //初始化
    public static function init($fd,$path)
    {
         self::$connections[$fd]['path'] = $path;
    }
    //获取fd的信息
    public static function get($fd)
    {
         return self::$connections[$fd];
    }
    //删除fd的信息
    public static function del($fd)
    {
        unset(self::$connections[$fd]);
    }
}// class end