<?php

/* Study\Coroutine::create(function(){
    echo 1;
});
 */


function task($a, $b)
{
    echo $a . PHP_EOL;
    echo $b . PHP_EOL;
}

Study\Coroutine::create('task', 'a', 'b');
