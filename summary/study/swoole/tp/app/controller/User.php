<?php
/**
 * Created by PhpStorm.
 * User: zpw
 * Date: 2023/7/21
 * Time: 21:21
 */

namespace app\controller;


use app\common\service\rabbitmq\Producer;

class User extends Base
{
    /**
     * 用户埋点记录
     * @return string
     */
    public function record()
    {
        $producer = new Producer();
        $data = [
            'message_type' => 2,
            'order_id' => 3,
            'user_id' => 3,
            'message' => "发送的消息内容：您的快递已到的配送站。"
        ];
        $producer->send($data);
        return "success";
    }

}// class end