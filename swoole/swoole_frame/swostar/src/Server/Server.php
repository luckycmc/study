<?php
namespace SwoStar\Server;

use SwoStar\Model\Db;
use SwoStar\Model\ManagerDB;
use SwoStar\Model\ManagerRedis;
use SwoStar\Model\Re;
use SwoStar\RPC\Rpc;
use SwoStar\Supper\Inotify;
use Swoole\Server as SwooleServer;
use SwoStar\Foundation\Application;

/**
 * 所有服务的父类， 写一写公共的操作
 */
abstract class Server
{
    // 属性
    /**
     * [protected description]
     * @var Swoole/Server
     */
    protected $swooleServer;

    protected $app ;

    protected $inotify = null;

    protected $port = 9000;

    protected $host = "0.0.0.0";

    protected $watchFile = false;
    /**
     * 用于记录系统pid的信息
     * @var string
     */
    protected $pidFile = "/runtime/swostar.pid";

    protected $config = [
        'task_worker_num' => 0,
        'worker_num'      => 1,
    ];
    /**
     * 用于记录pid的信息
     * @var array
     */
    protected $pidMap = [
        'masterPid'  => 0,
        'managerPid' => 0,
        'workerPids' => [],
        'taskPids'   => []
    ];
    /**
     * 注册的回调事件
     * [
     *   // 这是所有服务均会注册的时间
     *   "server" => [],
     *   // 子类的服务
     *   "sub" => [],
     *   // 额外扩展的回调函数
     *   "ext" => []
     * ]
     *
     * @var array
     */
    protected $event = [
        // 这是所有服务均会注册的时间
        "server" => [
            // 事件   =》 事件函数
            "start"        => "onStart",
            "managerStart" => "onManagerStart",
            "managerStop"  => "onManagerStop",
            "shutdown"     => "onShutdown",
            "workerStart"  => "onWorkerStart",
            "workerStop"   => "onWorkerStop",
            "workerError"  => "onWorkerError",
        ],
        // 子类的服务
        "sub" => [],
        // 额外扩展的回调函数
        // 如 ontart等
        "ext" => []
    ];
    //当前服务器的ip
    protected $server_host_ip ;
    //构造方法 启动服务
    public function __construct(Application $app )
    {
        $ips = swoole_get_local_ip();
        //获取当前服务器的ip
        $this->server_host_ip = $ips['ens33'];

        $this->app = $app;
        // 1. 创建 swoole server
        $this->createServer();

        // 2. 设置配置信息
        $this->swooleServer->set($this->config);
        // 3. 设置需要注册的回调函数
        $this->initEvent();
        // 4. 设置swoole的回调函数
        $this->setSwooleEvent();
        //启动服务器
        $this->start();

    }
    /**
     * 创建服务
     *
     */
    protected abstract function createServer();
    /**
     * 初始化监听的事件
     */
    protected abstract function initEvent();
    // 通用的方法

    public function start()
    {
        $config = app('config');
        // 2. 设置配置信息
        $this->swooleServer->set($this->config);
        //是否调用RPC服务
        if ($config->get('server.http.tcpable')) {

            new Rpc($this->swooleServer, $config->get('server.http.rpc'));
        }
        // 5. 启动
        $this->swooleServer->start();
    }
    //设置相应的配置信息
    public function initSetting()
    {
        $config = app('config');
        $this->port = $config->get('http.port');
        $this->host = $config->get('http.host');

    }
    /**
     * 设置swoole的回调事件
     */
    protected function setSwooleEvent()
    {
        foreach ($this->event as $type => $events)
        {
            foreach ($events as $event => $func)
            {
                //设置对应的回调函数
                $this->swooleServer->on($event, [$this, $func]);
            }
        }
    }
    //监控文件
    protected function watchEvent()
    {
        return function($event){
            $action = 'file:';
            switch ($event['mask']) {
                case IN_CREATE:
                    $action = 'IN_CREATE';
                    break;

                case IN_DELETE:
                    $action = 'IN_DELETE';
                    break;
                case \IN_MODIFY:
                    $action = 'IN_MODIF';
                    break;
                case \IN_MOVE:
                    $action = 'IN_MOVE';
                    break;
            }
            // 服务重启
            $this->swooleServer->reload();
        };
    }
    // 回调方法
    public function onStart(SwooleServer $server)
    {
        echo "| Swoole is startting.........".PHP_EOL;
        echo "| Swoole Server Version is :".swoole_version().PHP_EOL;
        echo "| Swoole Port is ".$this->port.PHP_EOL;
        echo "| Current local Ip is ".$this->server_host_ip.':'.$this->port.PHP_EOL;
        echo "| #####master#### :".$server->master_pid.PHP_EOL;
        echo "| swoole_cpu_num ".swoole_cpu_num().PHP_EOL;
        echo "| 主进程id 文件".SWOSTAR_ROOT.$this->pidFile.PHP_EOL;
        //记录主进程id
        file_put_contents(SWOSTAR_ROOT.$this->pidFile,$server->master_pid);
        //设置主进程名称
        swoole_set_process_name("master Swostar");

        $this->pidMap['masterPid'] = $server->master_pid;
        $this->pidMap['managerPid'] = $server->manager_pid;
        // 启动监控文件
        if ($this->watchFile ) {
            $this->inotify = new Inotify($this->app->getBasePath(), $this->watchEvent());
            $this->inotify->start();
        }
    }
    //管理进程启动时
    public function onManagerStart(SwooleServer $server)
    {
        echo "| #####manager#### :".$server->manager_pid.PHP_EOL;
        swoole_set_process_name("manager Swostar");
    }
    public function onManagerStop(SwooleServer $server)
    {

    }
    public function onShutdown(SwooleServer $server)
    {

    }
    public function onWorkerStart(SwooleServer $server, int $worker_id)
    {
        echo "| #####worker_id{$server->worker_pid}#### :".$server->worker_pid.PHP_EOL;
        //设置工作进程名称
        swoole_set_process_name("worker Swostar{$server->worker_pid}");
        //保留进程ids
        $this->pidMap['workerPids'] = [
            'id'  => $worker_id,
            'pid' => $server->worker_id
        ];
        echo str_repeat('----',15).PHP_EOL;
        /******************启动ORM start*******************/
         //加载Mysql数据库配置
         Db::getInstance()->set(ManagerDB::getInstance()->init());
         //记载redis数据库配置
         Re::getInstance()->set(ManagerRedis::getInstance()->init());
        /******************启动ORM  end*******************/
    }
    //工作进程停止时触发
    public function onWorkerStop(SwooleServer $server, int $worker_id)
    {

    }
    // 工作进程出现错误
    public function onWorkerError(SwooleServer $server, int $workerId, int $workerPid, int $exitCode, int $signal)
    {
    }

    // GET | SET

    /**
     * @param array
     *
     * @return static
     */
    public function setEvent($type, $event)
    {
        // 暂时不支持直接设置系统的回调事件
        if ($type == "server") {
            return $this;
        }
        $this->event[$type] = $event;
        return $this;
    }

    /**
     * @return array
     */
    public function getConfig(): array
    {
        return $this->config;
    }

    /**
     * @param array $config
     *
     * @return static
     */
    public function setConfig($config)
    {
        $this->config = array_map($this->config, $config);
        return $this;
    }

}// class end
