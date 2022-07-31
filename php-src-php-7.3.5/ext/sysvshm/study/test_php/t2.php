<?php
function task($n, $arg)
{
    echo "coroutine [$n]" . PHP_EOL;
    Study\Coroutine::yield();
    echo $arg . PHP_EOL;
}


$cid1 = Study\Coroutine::create('task', 1, 'a');
echo "main coroutine----1" . PHP_EOL;
$cid2 = Study\Coroutine::create('task', 2, 'b');
echo "main coroutine-----2" . PHP_EOL;
echo "main coroutine end......." . PHP_EOL;

echo "resume----------------------------".PHP_EOL;
Study\Coroutine::resume($cid1); //恢复协程1
Study\Coroutine::resume($cid2); //恢复协程2

if (Study\Coroutine::isExist($cid1))
{
    echo "coroutine [{$cid1}] is existent\n";
}
else
{
    echo "coroutine [{$cid1}] is non-existent\n";
}

