<?php
// rabbitmq 配置信息
return [
    'host'=>'192.168.17.140',
    'port'=>'5672',
    'user'=>'guest',
    'password'=>'guest',
    'vhost'=>'/',
    'exchange_name' => 'hello',
    'queue_name' => 'hello',
    'route_key' => 'hello',
    'consumer_tag' => 'consumer',
];