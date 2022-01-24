<?php
/**
 * Created by PhpStorm.
 * User: zpw
 * Date: 2020/10/11
 * Time: 16:48
 */

namespace SwoStar\RPC;

use Swoole\Server;
use SwoStar\Console\Input;

class Rpc
{
    protected $host;
    protected $port;

    public function __construct(Server $server,$config)
    {
         //添加另外一个监听端口
         $listen = $server->listen($config['host'],$config['port'],SWOOLE_SOCK_TCP);
         $listen->set($config['swoole']);
         //设置对应的事件
         $listen->on('connect', [$this, 'connect']);
         $listen->on('receive', [$this, 'receive']);
         $listen->on('close', [$this, 'close']);
         //打印输出对应的信息
         Input::info('tcp监听的地址: '.$config['host'].':'.$config['port'] );

    }
    public function connect($serv, $fd)
    {
        dd("超管查房");
    }

    public function receive($serv, $fd, $from_id, $data)
    {
        $serv->send($fd, 'Swoole: '.$data);
        $serv->close($fd);
    }

    public function close($serv, $fd)
    {
        echo "Client: Close.\n";
    }
}// class end