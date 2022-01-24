<?php
namespace SwoStar\Routes;

use SwoStar\Console\Input;

class Route
{
    protected static $instance = null;
    // 路由本质实现是会有一个容器在存储解析之后的路由
    protected $routes = [];

    // 定义了访问的类型
    protected $verbs = ['GET', 'POST', 'PUT', 'PATCH', 'DELETE'];
    // 记录路由的文件地址
    protected $routeMap = [];
    // 记录请求的方式
    // 语文叫铺垫 =》 websocket
    protected $method = null;

    protected $flag = null;

    protected function __construct( )
    {
        $this->routeMap = [
            'Http'      => app()->getBasePath().'/route/http.php',
            'WebSocket' => app()->getBasePath().'/route/web_socket.php',
        ];
    }

    public static function getInstance()
    {
        if (\is_null(self::$instance)) {
            self::$instance = new static();
        }
        return self::$instance ;
    }

    public function get($uri, $action)
    {
        return $this->addRoute(['GET'], $uri, $action);
    }

    public function post($uri, $action)
    {
        return $this->addRoute(['POST'], $uri, $action);
    }

    public function any($uri, $action)
    {
        return $this->addRoute(self::$verbs, $uri, $action);
    }

    public function wsController($uri, $controller)
    {
        $actions = [
            'open',
            'message',
            'close',
        ];
        foreach ($actions as $key => $action) {
            $this->addRoute([$action], $uri, $controller.'@'.$action);
        }
    }
    /**
     * 注册路由
     * @param array $methods [description]
     * @param [type] $uri     [description]
     * @param [type] $action  [description]
     */
    protected function addRoute($methods, $uri, $action)
    {
        foreach ($methods as $method )
        {
            $this->routes[$this->flag][$method][$uri] = $action;
        }
        return $this;
    }
    /**
     * 根据请求校验路由，并执行方法
     * @return [type] [description]
     */
    public function match($path, $param = [])
    {
        // 作业 1. 实现校验

        /*
        本质就是一个字符串的比对

        1. 获取请求的uripath
        2. 根据类型获取路由
        3. 根据请求的uri 匹配 相应的路由；并返回action
        4. 判断执行的方法的类型是控制器还是闭包
           4.1 执行闭包
           4.2 执行控制器
        */
        $action = null;
        //print_r($this->routes[$this->flag]);
        //更具请求查找对应的路由文件
        foreach ($this->routes[$this->flag][$this->method] as $uri => $value)
        {

            $uri = ($uri && substr($uri,0,1)!='/') ? "/".$uri : $uri;

            if ($path === $uri) {  //查找到对应的路由
                $action = $value;
                break;
            }
        }
        //查找匹配到的路由文件
        if (!empty($action)) {
            //执行对应的额方法
            return $this->runAction($action, $param);
        }
        //输出相应信息
        Input::info('没有找到方法');

        return "404";

    }
    //具体执行的方法
    private function runAction($action, $param = null)
    {
        //闭包函数的处理
        if ($action instanceof \Closure) {

            return $action(...$param);
        } else {

            // 控制器解析
            $namespace = "\\App\\".$this->flag."\\Controller\\";
            // IndexController@dd
            $arr = \explode("@", $action);
            $controller = $namespace.$arr[0];
            $class = new $controller();
            return $class->{$arr[1]}(...$param);
        }

    }
    // 注册路由文件
    public function registerRoute()
    {
        foreach ($this->routeMap as $key => $path) {
            $this->flag = $key;
            require_once $path;
        }
        return $this;
    }
   //设置对应的方法
    public function setMethod($method)
    {
        $this->method = $method;
        return $this;
    }
    //设置服务器模式
    public function setFlag($flag)
    {
        $this->flag = $flag;
        return $this;
    }
    //获取路由方式
    public function getRoutes()
    {
        return $this->routes;
    }

} //class end
