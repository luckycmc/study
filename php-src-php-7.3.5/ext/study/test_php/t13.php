<?php

//绑定服务器进行压测
study_event_init();

$cid = Sgo(function ()
{
    $serv = new Study\Coroutine\Server("0.0.0.0", 8080);
    while (1)
    {
        $connfd = $serv->accept();
        $buf = $serv->recv($connfd);
        $responseStr = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: close\r\nContent-Length: 11\r\n\r\nhello world\r\n";
        $serv->send($connfd, $responseStr);
        $serv->close($connfd);
    }
    $cid = Sco::getCid();
    echo $cid.PHP_EOL;
});

study_event_wait();

