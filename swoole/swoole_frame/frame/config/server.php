<?php
//服务器的相关配置信息
return [

    'http'=>[
        'host' => '0.0.0.0',
        'port' => 9000,
        'swoole' => [

        ],

        'tcpable' => 1, // 1为开启， 0 为关闭

        'rpc' => [
            'host' => '127.0.0.1',
            'port' => 8000,
            'swoole' => [
                'worker_num' => 1
            ]
        ]
    ],
    //'server_type' => 'WebScoket',
    'server_type'  => 'Http'  //开启http 服务器
    // ...
];
