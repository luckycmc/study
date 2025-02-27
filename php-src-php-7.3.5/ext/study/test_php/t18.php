<?php
/**
 * 实现socket 高性能服务器
 */
study_event_init();

Sgo(function ()
{
    $socket = new Study\Coroutine\Socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    $socket->bind("127.0.0.1", 8080);
    $socket->listen();
    while (true) {
        $conn = $socket->accept();
        Sgo(function () use($conn)
        {
            $data = $conn->recv();
            $responseStr = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: close\r\nContent-Length: 11\r\n\r\nhello world\r\n";
            $conn->send($responseStr);
            $conn->close();
        });
    }
});

study_event_wait();
