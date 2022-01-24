#!/usr/bin/env php
<?php
//
defined('SWOSTAR_ROOT') or define('SWOSTAR_ROOT',dirname(__DIR__));

require dirname(__DIR__).'/vendor/autoload.php';
//echo dirname(__DIR__).'/vendor/autoload.php'.PHP_EOL;
//启动服务
(new \SwoStar\Foundation\Application(dirname(__DIR__)))->run($argv);
//\SwoStar\Commands\Command::run();
