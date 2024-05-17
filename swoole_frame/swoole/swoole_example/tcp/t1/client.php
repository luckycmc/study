<?php

/**
 * Created by PhpStorm.
 * User: f
 * Date: 2021/7/30
 * Time: 14:54
 */
class client
{
    public function send($msg)
    {

        $client = new swoole_client(SWOOLE_SOCK_TCP);
        //连接到服务器
        if (!$client->connect('127.0.0.1', 9501, 0.5))
        {
            $this->write("connect failed.");
        }
        //向服务器发送数据
        if (!$client->send($msg))
        {
            $this->write("send ".$msg." failed.");
        }
        //关闭连接
        $client->close();
    }

    private function write($str)
    {
        $path = "/sys.log";

        $str = "[".date("Y-m-d H:i:s")."]".$str;

        $str .= PHP_EOL;

        file_put_contents($path,$str,FILE_APPEND);
    }

}//class end