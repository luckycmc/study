<?php
function task($a,$b)
{   
    echo $a . PHP_EOL;
    echo $b . PHP_EOL;

    echo "study extension success\n";
}

Study\Coroutine::create('task', 'a', 'b');