<?php
// 应用公共文件

function getRedis()
{
    $redis = new \Redis();
    $redis->connect('127.0.0.1',6379);
    return $redis;
}