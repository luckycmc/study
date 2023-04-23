<?php
/*****
 * 服务器的设置 和启动
 */

class Server
{    
     protected $server;
     protected $port = 9502;   // 端口号
     protected $IP = '0.0.0.0'; // 对应的IP

     public function __construct()
     {     
           //创建PHP 服务器
           $this->server = new Swoole\Http\Server($this->IP,$this->port);
           // 设置php服务器属性
           $this->server->set([
                // 设置工作进程的个数
                'worker_num' => swoole_cpu_num() * 2, // swoole扩展内置函数
                'document_root' => ROOT_PATH.'/html/', // v4.4.0以下版本, 此处必须为绝对路径
                'enable_static_handler' => true,
           ]);
           //设置 回调事件 worker进程创建成功后会回调这个方法 当前方法必须为 public 属性
           $this->server->on('WorkerStart',([$this,'WorkerStart']));
           // 注册请求事件 当有http请求的时候 会触发这个回调函数
           $this->server->on('request',([$this,'onRequest']));
     }

    /**
     * worker 进程启动
     * @param \Swoole\Http\Server $server 对应的server对象
     * @param $worker_id  // 当前worker 进程的id
     */
    public function WorkerStart(\Swoole\Http\Server $server, $worker_id)
    {
         //echo $worker_id.PHP_EOL;
         //注册自动加载函数
         spl_autoload_register('Loader::autoload',true,true);
    }

    /**
     * 有客户端请求的时候会触发
     * @param \Swoole\Http\Request $request
     * @param \Swoole\Http\Response $response
     */
     public function onRequest(\Swoole\Http\Request $request,\Swoole\Http\Response $response)
     {
           $this->request($request,$response);
     }
     //处理request请求
     public function request(\Swoole\Http\Request $request,\Swoole\Http\Response $response)
     {
         // 获取请求的
         $pathInfo = explode('/', ltrim($request->server['path_info'], '/'));
         if (count($pathInfo) <= 1){
             $pathInfo[0] = 'Index';
             $pathInfo[1] = 'Index';
         }
         //获取对应的控制器和方法
         $controller = isset($pathInfo[0]) ?ucfirst($pathInfo[0]): 'Index';
         $method     = isset($pathInfo[1]) ?ucfirst($pathInfo[1]): 'index';
         //处理请求和返回数据
         try {
             $class = "{$controller}"; // 当前的控制器
             $result = (new $class)->{$method}(); //对应请求的方式
             $response->end($result); // 把结果相应给请求客户端
         } catch (\Throwable $e) { //异常处理

             $responseContent = $e->getFile().':'.$e->getLine().':'.$e->getMessage();
             $response->end($responseContent);
         }
     }

    /**
     * /服务器启动
     */
     public function startServer()
     {     
            //调用http 服务器启动
           $this->server->start();
     }

}// cklass end