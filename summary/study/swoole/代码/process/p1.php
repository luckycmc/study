<?php
use Swoole\Process;
//查看进程id 读数据是共享 写数据时复制
function showPId()
{
    echo "pid=".posix_getpid().PHP_EOL;
}
showPId();
//进程绑定对应的函数入口去指行对应 的任务
$process = new Process(function (){
    showPId();
});
//启动进程
$process->start();