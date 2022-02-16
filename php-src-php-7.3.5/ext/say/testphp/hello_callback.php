<?php
/* function fun1() {
    for ($i = 0; $i < 5; $i++) {
        echo 'fun1:'.$i."\n";
    }
    return 'call end';
}

echo hello_callback('fun1'); */

function fun1() {
    for ($i = 0; $i < 5; $i++) {
        usleep(122);
        echo 'fun1:'.$i."\n";
    }
}

function fun2() {
    for ($i = 0; $i < 5; $i++) {
        usleep(123);
        echo 'fun2:'.$i."\n";
    }
}

hello_thread('fun1', 'fun2');
echo 'after 多并发\n';