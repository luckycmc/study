<?php
declare(strict_types=1);
/**
 * rabbitMQ的配置
 * Created by PhpStorm.
 * User: zpw
 * Date: 2023/9/9
 * Time: 17:42
 */

return [
    'default' => [
        'host' => '192.168.17.140',
        'port' => 5672,
        'user' => 'guest',
        'password' => 'guest',
        'vhost' => '/',
        'concurrent' => [
            'limit' => 1,
        ],
        'pool' => [
            'connections' => 10,
        ],
        'params' => [
            'insist' => false,
            'login_method' => 'AMQPLAIN',
            'login_response' => null,
            'locale' => 'en_US',
            'connection_timeout' => 3.0,
            'read_write_timeout' => 6.0,
            'context' => null,
            'keepalive' => false,
            'heartbeat' => 3,
            'close_on_destruct' => false,
        ],
    ],
    'pool2' => [

    ],
];
