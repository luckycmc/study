<?php
namespace SwoStar\Message\Http;

use Swoole\Http\Request as SwooleRequest;

class Request
{

    protected $method;

    protected $uriPath;

    protected $swooleRequest;

    public function getMethod()
    {
        return $this->method;
    }

    public function getUriPath()
    {
        return $this->uriPath;
    }
    /**
     * [init description]
     * @param  SwooleRequest $request [description]
     * @return Request                 [description]
     */
    public static function init(SwooleRequest $request)
    {
        $self = app('httpRequest');

        $self->swooleRequest = $request;
        $self->server = $request->server;
        //获取请求方式
        $self->method = $request->server['request_method'] ?? '';
        //获取请求路径
        $self->uriPath = $request->server['request_uri'] ?? '';
        return $self;
    }
    //获取get 参数
    public function get()
    {

    }
    public function post()
    {

    }
    public function input()
    {

    }

}//  class end
