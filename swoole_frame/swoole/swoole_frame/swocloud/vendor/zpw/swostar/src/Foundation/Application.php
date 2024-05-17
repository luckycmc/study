<?php
/**
 * Created by PhpStorm.
 * User: zpw
 * Date: 2020/9/19
 * Time: 16:06
 */

namespace SwoStar\Foundation;


use SwoStar\Commands\Command;
use SwoStar\Container\Container;
use SwoStar\Event\Event;
use SwoStar\Routes\Route;
use SwoStar\Server\Http\HttpServer;
use SwoStar\Server\WebSocket\WebSocketServer;

class Application extends Container
{
    const SWOSTAR_WELCOME = "
      _____                     _____     ___
     /  __/             ____   /  __/  __/  /__   ___ __    __  __
     \__ \  | | /| / / / __ \  \__ \  /_   ___/  /  _`  |  |  \/ /
     __/ /  | |/ |/ / / /_/ /  __/ /   /  /_    |  (_|  |  |   _/
    /___/   |__/\__/  \____/  /___/    \___/     \___/\_|  |__|
    ";
    protected $basePath = "";
    //首先会被触发
    public function __construct($path = null)
    {

        if(!empty($path)){

             $this->setBasePath($path);
        }
        //注册容器服务
        $this->registerBaseBindings();

        //初始化路由加载
        $this->init();

        //输出服务信息
        dd(self::SWOSTAR_WELCOME.PHP_EOL,'启动项');
    }
    //初始化路由
    public function init()
    {
        //绑定路由实例对象
        $this->bind('route', Route::getInstance()->registerRoute());
        $this->bind('event', $this->registerEvent());
    }
    /**
     * 注册框架事件
     * @return Event
     */
    public function registerEvent()
    {

        $event = new Event();

        $files = scandir($this->getBasePath().'/app/Listener');

        // 2. 读取文件信息
        foreach ($files as $key => $file) {
            if ($file === '.' || $file === '..') {
                continue;
            }
            // $file => StartListener.php

            $class = 'App\\Listener\\'.\explode('.', $file)[0];

            if (\class_exists($class)) {
                $listener = new $class;
                $event->register($listener->getName(), [$listener, 'handler']);
            }
        }

        return $event;
    }
    /**
     * 注册绑定对象
     */
    public function registerBaseBindings()
    {
        self::setInstance($this);

        $binds = [
            // 标识  ， 对象
            'config'      => (new \SwoStar\Config\Config()),
            'index'       => (new \SwoStar\Index()),
            'httpRequest' => (new \SwoStar\Message\Http\Request()),
        ];

        foreach ($binds as $key => $value) {
            $this->bind($key, $value);
        }
    }
    //启动程序
    public function run($arg)
    {
        $server = null;
        //获取启动服务器的类型
        $ServerType = app('config')->get('server.server_type');
        //执行对应的命令服务
        switch ($ServerType)
        {
            case 'Http':
                $server = new HttpServer($this); // http 服务器
                break;
            case 'WebScoket':
                $server = new WebSocketServer($this); // webscoket 服务器
                break;
            default:
                //启动tcp 服务器 暂时不错处理
                exit("error commend");
        }
        $server->watchFile(true);
        //启动服务
        $server->start();
    }
    //设置基础目录
    public function setBasePath($path)
    {
        $this->basePath = \rtrim($path, '\/');
    }
    //获取设置的目录
    public function getBasePath()
    {
        return $this->basePath;
    }

}// class end