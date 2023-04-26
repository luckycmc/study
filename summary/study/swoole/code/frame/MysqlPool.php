<?php

use Swoole\Coroutine\Channel;
use Swoole\Coroutine\MySQL;
class MysqlPool
{
    private $min; // 最小连接数
    private $max; // 最大连接数
    private $count; // 当前连接数
    private $connections; // 连接池
    protected $freeTime; // 用于空闲连接回收判断
    public static $instance;
    /**
     * MysqlPool constructor.
     */
    public function __construct()
    {
        $this->min = 5;
        $this->max = 10;
        $this->freeTime = 10 * 3600;
        $this->connections = new Channel($this->max + 1);
    }
    /**
     * @return MysqlPool
     */
    public static function getInstance()
    {
        if (is_null(self::$instance)) {
            self::$instance = new self();
        }
        return self::$instance;
    }
    /**
     * 创建连接
     * @return MySQL
     */
    protected function createConnection()
    {
        $conn = new MySQL();
        $conn->connect([
            'host' => '127.0.0.1',
            'port' => '3306',
            'user' => 'root',
            'password' => 'root',
            'database' => 'study',
            'timeout'  => 5
        ]);
        return $conn;
    }
    /**
     * 创建连接对象
     * @return array|null
     */
    protected function createConnObject()
    {
        $conn = $this->createConnection();
        return $conn ? ['last_used_time' => time(), 'conn' => $conn] : null;
    }
    /**
     * 初始化连接
     * @return $this
     */
    public function init()
    {
        for ($i = 0; $i < $this->min; $i++) {
            $obj = $this->createConnObject();
            $this->count++;
            $this->connections->push($obj);
        }
        return $this;
    }
    /**
     * 获取连接
     * @param int $timeout
     * @return mixed
     */
    public function getConn($timeout = 3)
    {
        if ($this->connections->isEmpty()) {
            if ($this->count < $this->max) {
                $this->count++;
                $obj = $this->createConnObject();
            } else {
                $obj = $this->connections->pop($timeout);
            }
        } else {
            $obj = $this->connections->pop($timeout);
        }
        return $obj['conn']->connected ? $obj['conn'] : $this->getConn();
    }
    /**
     * 回收连接
     * @param $conn
     */
    public function recycle($conn)
    {
        if ($conn->connected) {
            $this->connections->push(['last_used_time' => time(), 'conn' => $conn]);
        }
    }
    /**
     * 回收空闲连接
     */
    public function recycleFreeConnection()
    {
        // 每 2 分钟检测一下空闲连接
        swoole_timer_tick(2 * 60 * 1000, function () {
            if ($this->connections->length() < intval($this->max * 0.5)) {
                // 请求连接数还比较多，暂时不回收空闲连接
                return;
            }
            while (true) {
                if ($this->connections->isEmpty()) {
                    break;
                }
                $connObj = $this->connections->pop(0.001);
                $nowTime = time();
                $lastUsedTime = $connObj['last_used_time'];
                // 当前连接数大于最小的连接数，并且回收掉空闲的连接
                if ($this->count > $this->min && ($nowTime - $lastUsedTime > $this->freeTime)) {
                    //关闭连接
                    $connObj['conn']->close();
                    $this->count--;
                } else {
                    $this->connections->push($connObj);
                }
            }
        });
    }
}

/**
 *
 *  使用案例
  $httpServer = new swoole_http_server('127.0.0.1',9501);
    $httpServer->set(['work_num' => 1]);
    $httpServer->on('WorkerStart', function ($request, $response) {
        MysqlPool::getInstance()->init()->recycleFreeConnection();
    });
    $httpServer->on('Request', function ($request, $response){
        $conn = MysqlPool::getInstance()->getConn();
        $conn->query('SELECT * FROM fa_admin WHERE id=1');
        MysqlPool::getInstance()->recycle($conn);
    });
    $httpServer->start();
 * */
