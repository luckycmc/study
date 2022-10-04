<?php
//协成连接池的数据信息
return [

    'host'      => '127.0.0.1', //服务器地址
    'port'      => 3306,    //端口
    'user'      => 'root',  //用户名
    'password'  => 'root',  //密码
    'charset'   => 'utf8',  //编码
    'database'  => 'user_chat',  //数据库名
    'prefix'    => 'tp_',  //表前缀
    'poolMin'   => 5, //空闲时，保存的最大链接，默认为5
    'poolMax'   => 50,    //地址池最大连接数，默认1000
    'clearTime' => 60000, //清除空闲链接定时器，默认60秒，单位ms
    'clearAll'  => 300000,  //空闲多久清空所有连接，默认5分钟，单位ms
    'setDefer'  => true,     //设置是否返回结果,默认为true,
];
