<?php
namespace SwoCloud;

use Swoole\Server as SwooleServer;

/**
 * 所有服务的父类， 写一写公共的操作
 */
abstract class Server
{
    // 属性
    /**
     *
     * @var Swoole/Server
     */
    protected $swooleServer;

    protected $port = 9500;

    protected $host = "0.0.0.0";

    protected $watchFile = false;

    protected $server_host_ip;
    /**
     * 这是swoole服务的配置
     * @var [type]
     */
    protected $config = [
        'task_worker_num' => 0,
        'worker_num'      => 2,
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
     * 记录id 存放的更目录
     * @var
     */
    protected $pidDir ;
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

    public function __construct()
    {
        $this->pidDir = SWOSTAR_ROOT.'/runtime/';
        //创建进程存放的目录
        if(!file_exists($this->pidDir)){
             mkdir($this->pidDir);
        }
        $ips = swoole_get_local_ip();
        //获取当前服务器的ip
        $this->server_host_ip = $ips['ens33'];
        // 1. 创建 swoole server
        $this->createServer();
        // 3. 设置需要注册的回调函数
        $this->initEvent();
        // 4. 设置swoole的回调函数
        $this->setSwooleEvent();
    }
    /**
     * 创建服务
     */
    protected abstract function createServer();
    /**
     * 初始化监听的事件
     */
    protected abstract function initEvent();
    // 通用的方法

    public function start()
    {

        // 2. 设置配置信息
        $this->swooleServer->set($this->config);
        // 5. 启动
        $this->swooleServer->start();
    }

    /**
     * 设置swoole的回调事件
     *
     */
    protected function setSwooleEvent()
    {
        foreach ($this->event as $type => $events)
        {
            foreach ($events as $event => $func)
            {
                $this->swooleServer->on($event, [$this, $func]);
            }
        }
    }

    // 回调方法
    public function onStart(SwooleServer $server)
    {
          file_put_contents($this->pidDir.'master_pid.pid',$server->master_pid);
          $this->pidMap['masterPid'] = $server->master_pid;
    }

    public function onManagerStart(SwooleServer $server)
    {
        file_put_contents($this->pidDir.'manager_pid.pid',$server->manager_pid);
        $this->pidMap['managerPid'] = $server->manager_pid;
    }
    public function onManagerStop(SwooleServer $server)
    {

    }
    public function onShutdown(SwooleServer $server)
    {

    }
    public function onWorkerStart(SwooleServer $server, int $worker_id)
    {
        file_put_contents($this->pidDir.'worker_pid.pid',$server->worker_pid."\n",FILE_APPEND);
        $this->pidMap['workerPids'][] = $server->worker_pid;
    }
    public function onWorkerStop(SwooleServer $server, int $worker_id)
    {

    }
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
        $this->config = array_merge($this->config, $config);

        return $this;
    }

    public function watchFile($watchFile)
    {
        $this->watchFile = $watchFile;
    }
}
