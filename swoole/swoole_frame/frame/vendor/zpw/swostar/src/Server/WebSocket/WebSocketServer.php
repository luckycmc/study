<?php
namespace SwoStar\Server\WebSocket;

use SwoStar\Console\Input;

use SwoStar\Server\Http\HttpServer;
use Swoole\Http\Request;
use Swoole\Http\Response;
use Swoole\WebSocket\Server as SwooleServer;

class WebSocketServer extends HttpServer
{
    //创建主服务
    public function createServer()
    {
        $this->swooleServer = new SwooleServer($this->host, $this->port);

        Input::info('WebSocket server 访问 : ws://'.$this->server_host_ip.':'.$this->port );
    }
    // 事件初始化
    protected function initEvent()
    {
        $this->setEvent('sub', [
            'request' => 'onRequest',
            'open' => "onOpen",
            'message' => "onMessage",
            'close' => "onClose",
        ]);
        // 判断是否自定义握手的过程
        //( ! $this->app->make('config')->get('server.ws.is_handshake'))?: $event['handshake'] = 'onHandShake';

       // $this->setEvent('sub', $event);
    }
    public function onHandShake(Request $request, Response $response)
    {
        $this->app->make('event')->trigger('ws.hand', [$this, $request, $response]);
    }
    // 当客户端连接进来的时候
    public function onOpen(SwooleServer $server, \Swoole\Http\Request $request)
    {
        // 需要获取访问的地址？
        Connections::init($request->fd, $request->server['path_info']); //记录用户访问信息
        // 解析给对应的控制器去处理
        app('route')->setFlag('WebSocket')->setMethod('open')->match($request->server['path_info'], [$server, $request]);
    }
    //消息推送
    public function onMessage(SwooleServer $server, $frame)
    {

        $path = (Connections::get($frame->fd))['path'];

        app('route')->setFlag('WebSocket')->setMethod('message')->match($path, [$server, $frame]);
    }
    // 客户端断开连接
    public function onClose($server, $fd)
    {
        $path = (Connections::get($fd))['path'];
    
        app('route')->setFlag('WebSocket')->setMethod('close')->match($path, [$server, $fd]);

        Connections::del($fd);
    }

}// class end
