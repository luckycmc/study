<?php
/*****
 * 服务器的设置 和启动
 */

class Server
{    
     protected $server;
     protected $port = 9501;
     protected $IP = '0.0.0.0';

     public function __construct()
     {     
           //创建PHP 服务器
           $this->server = new Swoole\Http\Server($this->IP,$this->port);
           // 设置php服务器属性
           $this->server->set([
                'worker_num' => swoole_cpu_num(),
           ]);
           //设置 回调事件
           $this->server->on('WorkerStart',([$this,'WorkerStart']));
           // 请求事件
           $this->server->on('request',([$this,'onRequest']));
     }
     //worker 进程启动
    public function WorkerStart(\Swoole\Http\Server $server, $worker_id)
    {
         //echo $worker_id.PHP_EOL;
         //注册自动加载函数
         spl_autoload_register('Loader::autoload',true,true);
    }
     //worker 进程启动
     public function onRequest(\Swoole\Http\Request $request,\Swoole\Http\Response $response)
     {
           $this->request($request,$response);
     }
     //处理request请求
     public function request(\Swoole\Http\Request $request,\Swoole\Http\Response $response)
     {
         $pathInfo = explode('/', ltrim($request->server['path_info'], '/'));
         //获取对应的控制器和方法
         $controller = isset($pathInfo[0]) ?ucfirst($pathInfo[0]): 'Index';
         $method     = isset($pathInfo[1]) ?ucfirst($pathInfo[1]): 'index';
         //处理请求和返回数据
         try {
             $class = "{$controller}";
             $result = (new $class)->{$method}();
             $response->end($result);
         } catch (\Throwable $e) {
             $response->end($e->getMessage());
         }
     }
    //服务器启动
     public function startServer()
     {     
            //调用http 服务器启动
           $this->server->start();
     }

}// cklass end