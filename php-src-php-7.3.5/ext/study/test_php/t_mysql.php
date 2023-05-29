<?php

study_event_init();

zpw_go(function ()
{
    $server = new Study\Coroutine\Server("127.0.0.1", 8080);
    $server->set_handler(function (Study\Coroutine\Socket $conn) use($server) {
        $data = $conn->recv();
        $responseStr = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: close\r\nContent-Length: 11\r\n\r\nhello world\r\n";
        $conn->send($responseStr);
        $conn->close();
        //模拟0.5 IO阻塞
        Sco::sleep(0.5);
    });
    $server->start();
});

study_event_wait();