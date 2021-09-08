<?php
var_dump($argv);
$stdin = fopen("php://stdin", 'r');
echo "Master: ".fgets($stdin)."\n";

sleep(1);
ack_function(swoole_process $worker)
{
    $worker->exec('/usr/local/bin/php', array(__DIR__.'/test.php'));
}

echo "From Worker: ".$process->read();
$process->write("hello worker\n");
echo "From Worker: ".$process->read();

$ret = swoole_process::wait();
var_dump($ret);

