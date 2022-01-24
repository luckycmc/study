<?php
/**
 * Created by PhpStorm.
 * User: zpw
 * Date: 2020/9/19
 * Time: 16:25
 */

namespace SwoStar\Server\Http;

use SwoStar\Message\Http\Request;
use SwoStar\Routes\Route;
use SwoStar\Server\Server;

class HttpServer extends Server
{
     //实现基类中的抽象方法
     public function createServer()
     {
         $this->swooleServer = new \Swoole\Http\Server($this->host,$this->port);
     }
     //初始化注册事件
     protected function initEvent()
     {
        $this->setEvent('sub',[
            'request'=>'onRequest',
        ]);
     }
     //onRequest
    public function onRequest( \Swoole\Http\Request $request,\Swoole\Http\Response $response)
    {

        $httpRequest = Request::init($request);  //获取 http 对象
        //执行对应的相应控制器和对应的操作
        $return = Route::getInstance()->setFlag('Http')->setMethod($httpRequest->getMethod())->match($httpRequest->getUriPath());

        //相应给浏览器
        $response->header('Content-type', 'application/json;charset=utf-8');;
        $response->end($return);
    }

}// class end