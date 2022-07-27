<?php
function task($n, $arg)
{
    echo "coroutine [$n]" . PHP_EOL;
    Study\Coroutine::yield();
    echo $arg . PHP_EOL;
}


Study\Coroutine::create('task', 1, 'a');
echo "main coroutine----1" . PHP_EOL;
Study\Coroutine::create('task', 2, 'b');
echo "main coroutine-----2" . PHP_EOL;
