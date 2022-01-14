<?php
// /usr/local/php7.3/bin/php 7.3.5的PHP 启动


Study\Coroutine::create(function ($a, $b){
    echo $a . PHP_EOL;
    echo $b . PHP_EOL;
}, 'a', 'b');
