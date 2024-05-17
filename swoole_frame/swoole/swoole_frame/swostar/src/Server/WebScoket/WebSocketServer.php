<?php
/**
 * Created by PhpStorm.
 * User: zpw
 * Date: 2020/10/11
 * Time: 19:01
 */

namespace woStar\Server\WebSocket;
use Swoole\WebSocket\Server as SwooleServer;
use SwoStar\Server\Server;
use SwoStar\Console\Input;

class WebSocketServer extends Server
{
    //创建服务
    public function createServer()
    {
        $this->swooleServer = new SwooleServer($this->host, $this->port);
        Input::info('WebSocket server 访问 : ws://192.168.17.128:'.$this->port );
    }
    //初始化事件
    protected function initEvent()
    {
        $this->setEvent('sub', [
            'request' => 'onRequest',
            'open' => "onOpen",
            'message' => "onMessage",
            'close' => "onClose",
        ]);
    }
    //客户端进来的时候
   public function onOpen(SwooleServer $server, $request)
   {
        // 获取需要访问的地址
        Connections::init($request->fd,$request->server['path_info']);

        $return = app('route')->setFlag('WebSocket')->setMethod('open')->match($request->server['path_info'], [$server, $request]);
   }
   //发送消息
    public function onMessage(SwooleServer $server, $frame)
    {
        $path = (Connections::get($frame->fd))['path'];

        $return = app('route')->setFlag('WebSocket')->setMethod('message')->match($path, [$server, $frame]);
    }
    //关闭客户端
    public function onClose(SwooleServer $server, $fd)
    {
        $path = (Connections::get($fd))['path'];

        $return = app('route')->setFlag('WebSocket')->setMethod('close')->match($path, [$server, $fd]);

        Connections::del($fd);
    }
}// class end