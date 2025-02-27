<?php

use think\swoole\websocket\socketio\Handler;

return [
    'server'     => [
        'host'      => env('SWOOLE_HOST', '0.0.0.0'), // 监听地址
        'port'      => env('SWOOLE_PORT', 8080), // 监听端口
        'mode'      => SWOOLE_PROCESS, // 运行模式 默认为SWOOLE_PROCESS
        'sock_type' => SWOOLE_SOCK_TCP, // sock type 默认为SWOOLE_SOCK_TCP
        'options'   => [
            'pid_file'              => runtime_path() . 'swoole.pid',
            'log_file'              => runtime_path() . 'swoole.log',
            'daemonize'             => false,
            // Normally this value should be 1~4 times larger according to your cpu cores.
            'reactor_num'           => swoole_cpu_num(),
            'worker_num'            => swoole_cpu_num()*2,
            //'task_worker_num'       => swoole_cpu_num() * 0,
            'enable_static_handler' => true,
            'document_root'         => root_path('public'),
            'package_max_length'    => 20 * 1024 * 1024,
            'buffer_output_size'    => 10 * 1024 * 1024,
            'socket_buffer_size'    => 128 * 1024 * 1024,
        ],
    ],
    'websocket'  => [
        'enable'        => false,
        'handler'       => Handler::class,
        'ping_interval' => 25000,
        'ping_timeout'  => 60000,
        'room'          => [
            'type'  => 'table',
            'table' => [
                'room_rows'   => 4096,
                'room_size'   => 2048,
                'client_rows' => 8192,
                'client_size' => 2048,
            ],
            'redis' => [
                'host'          => '127.0.0.1',
                'port'          => 6379,
                'max_active'    => 3,
                'max_wait_time' => 5,
            ],
        ],
        'listen'        => [],
        'subscribe'     => [],
    ],
    'rpc'        => [
        'server' => [
            'enable'   => false,
            'port'     => 9000,
            'services' => [
            ],
        ],
        'client' => [
        ],
    ],
    //应用文件热更新配置 开始时不用 重启代码自动更新
    'hot_update' => [
        'enable'  => env('APP_DEBUG', true), //true 开启 false 关闭 线上要关闭
        'name'    => ['*.php'],   // php 后缀结尾的文件
        'include' => [app_path()],  // 监控目录
        'exclude' => [],
    ],
    //连接池
    'pool'       => [
        'db'    => [
            'enable'        => true,
            'max_active'    => 20, //最大数
            'min_active'    => 5,  //最小数
            'max_wait_time' => 5,
        ],
        'cache' => [
            'enable'        => true,
            'max_active'    => 10, //最大数
            'min_active'    => 5,  //最小数
            'max_wait_time' => 5,
        ],
        //自定义连接池
    ],
    //队列
    'queue'      => [
        'enable'  => false,
        'workers' => [],
    ],
    //全面协成话
    'coroutine'  => [
        'enable' => true,
        'flags'  => SWOOLE_HOOK_ALL,
    ],
    'tables'     => [],
    //每个worker里需要预加载以共用的实例
    'concretes'  => [],
    //重置器
    'resetters'  => [],
    //每次请求前需要清空的实例
    'instances'  => [],
    //每次请求前需要重新执行的服务
    'services'   => [],
    //redis 连接池 单独使用的

];
