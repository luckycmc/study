<?php

$t1 = time();

$cid = Sgo(function () {
    echo "before sleep" . PHP_EOL;
    SCo::sleep(10);
    echo "after sleep" . PHP_EOL;
});

echo "main co" . PHP_EOL;

SCo::scheduler();



