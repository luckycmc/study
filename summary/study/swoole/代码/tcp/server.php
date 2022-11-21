<?php

/**
 * 注意面向对象的写法方法的权限
 * 必须为public否则不能执行
 * Class Server
 */
class Server
{

    private $serv;

    /**
     *
     * Server constructor.
     */
    public function __construct()
    {
         $this->serv = new Swoole\Http\Server('127.0.0.1',9506);
         $this->serv->set([
             'worker_num' => swoole_cpu_num(), //开启2个worker进程
             'max_request' => 4,//每个worker进程 max_request设置为4次
         ]);
        /**********注册相应的事件 start***********/
        $this->serv->on('Start',[$this,'onStart']);
        $this->serv->on('Request',[$this,'onRequest']);
        /**********注册相应的事件 end***********/
        //监听tcp 9=9607
         $tcp = $this->serv->listen("0.0.0.0", 9507, SWOOLE_SOCK_TCP);
        //需要调用 set 方法覆盖主服务器的设置
        //默认新监听的端口 9999 会继承主服务器的设置，也是 HTTP 协议
        //需要调用 set 方法覆盖主服务器的设置
        //必须单独调用 port 对象的 set 方法和 on 方法设置新的协议才会启用新协议
        $tcp->set([
            'worker_num' => swoole_cpu_num(), //开启2个worker进程
        ]);
        /**********注册相应的事件 start***********/
        $tcp->on('Connect',[$this,'onConnect']);
        $tcp->on('Receive',[$this,'onReceive']);
        $tcp->on('Close',[$this,'onClose']);
        /**********注册相应的事件 end***********/
        //启动服务器
         $this->serv->start();
    }

    /**
     * 主进程启动时
     * @param \Swoole\Server $server
     */
    public function onStart(\Swoole\Server $server)
    {
        echo "#### onStart ####" . PHP_EOL;
        echo "swoole_cpu_num:". swoole_cpu_num(). PHP_EOL;
        echo "SWOOLE".SWOOLE_VERSION."服务器已经启动".PHP_EOL;
        echo "master_pid: ".$server->master_pid. PHP_EOL;
        echo "manager_pid: ".$server->manager_pid. PHP_EOL;
        echo "########".PHP_EOL .PHP_EOL;
    }
    //客户端连接服务器时
    public function onConnect(\Swoole\Server $server,$fd)
    {
        echo "#### onConnect ####" . PHP_EOL;
        echo "客户端:" . $fd . " 已连接" . PHP_EOL;
        echo "########" . PHP_EOL . PHP_EOL;
    }
    //接受客户端发送的数据
    public function onReceive(\Swoole\Server $server, $fd,$reactor_id,$data)
    {
        echo "#### onReceive ####" . PHP_EOL;
        echo "Msg:" . $data . PHP_EOL;
    }
    //客户端关闭数据
    public function onClose(\Swoole\Server $server,int $fd)
    {
        echo "fd $fd Client Close." . PHP_EOL;
    }
    //http的请求
    public function onRequest(\Swoole\Http\Request $request,\Swoole\Http\Response $response)
    {
         $response->end('hello world');
    }
}// class end

new Server();


