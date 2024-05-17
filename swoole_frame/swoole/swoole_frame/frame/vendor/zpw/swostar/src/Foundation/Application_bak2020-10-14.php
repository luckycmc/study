<?php
/**
 * Created by PhpStorm.
 * User: zpw
 * Date: 2020/9/19
 * Time: 16:06
 */

namespace SwoStar\Foundation;


use SwoStar\Container\Container;
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
    }
    /**
     * 注册绑定对象
     */
    public function registerBaseBindings()
    {
        self::setInstance($this);

        $binds = [
            // 标识  ， 对象
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
        //执行对应的命令服务
        switch ($arg[1]) {
            case 'http:start':
                $server = new HttpServer($this);
                break;
            case 'ws:start':
                $server = new WebSocketServer($this);
                break;
            default:
                exit("error commend");
        }
        $server->watchFile(true);
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