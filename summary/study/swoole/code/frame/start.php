#!/usr/bin/env php
<?php
// 框架启动
require './Server.php';
require './Loader.php';
//根目录常量
define('ROOT_PATH',dirname(__FILE__));

$server = new Server();
//启动服务器
$server->startServer();
