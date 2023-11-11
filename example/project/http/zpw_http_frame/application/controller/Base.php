<?php
namespace app\controller;


use Family\Pool\Context;

/**
 * controller 的基类
 * Class Base
 * @package controller
 */
class Base
{
    /**
     * 当前携协程的请求
     * @var \swoole_http_request
     */
    protected $request;

    /**
     * 初始化请求对象
     * Base constructor.
     */
    public function __construct()
    {
        //获取当前协程的上下文
        $context = Context::getContext();
        //获取当前的请求对象
        $this->request =$context->getRequest();
    }

}// class end