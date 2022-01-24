<?php
namespace App\WebSocket\Controller;

/**
 *
 */
class IndexController
{
    //连接服务端
    public function open(\Swoole\WebSocket\Server $server, \Swoole\Http\Request $request)
    {
        dd('indexController open');
    }
    // 给客户端推送数据
    public function message(\Swoole\WebSocket\Server $server,\Swoole\WebSocket\Frame $frame)
    {
        $server->push($frame->fd, "this is server ".$frame->data.' time: '.date('Y-m-d H:i:s',time()));
    }
    //客户端关闭触发
    public function close(\Swoole\WebSocket\Server $server, $fd)
    {
         var_dump($fd);
    }

}// class end
