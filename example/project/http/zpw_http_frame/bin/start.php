<?php
use Family\Family;

// 加载框架的启动项
require dirname(__DIR__) . DIRECTORY_SEPARATOR . 'framework' . DIRECTORY_SEPARATOR . 'Family' . DIRECTORY_SEPARATOR . 'Family.php';
//设置全面协程话
\Swoole\Runtime::enableCoroutine([$flags = SWOOLE_HOOK_ALL]);
//启动框架运行
Family::run();