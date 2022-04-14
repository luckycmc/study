<?php

function task($n, $arg)
{
    echo "coroutine [$n]" . PHP_EOL;
    Study\Coroutine::yield();
    echo $arg . PHP_EOL;  //后续不在执行
}

echo "main coroutine" . PHP_EOL;
Study\Coroutine::create('task', 1, 'a');
echo "main coroutine" . PHP_EOL;
Study\Coroutine::create('task', 2, 'b');
echo "main coroutine" . PHP_EOL;