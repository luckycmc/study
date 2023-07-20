<?php
/**
 * Created by PhpStorm.
 * User: zpw
 * Date: 2023/7/20
 * Time: 16:55
 */

namespace app\common\service\rabbitmq;


use PhpAmqpLib\Connection\AMQPStreamConnection;
use PhpAmqpLib\Message\AMQPMessage;

class Producer
{
   //连接
    protected $connection;
    //管道
    protected $channel;
    //配置内容
    protected $mqConfig;
    public function __construct()
    {
        $this->mqConfig = config('rabbitmq');
        //获取连接
        $this->connection = new AMQPStreamConnection(
            $this->mqConfig['host'],
            $this->mqConfig['port'],
            $this->mqConfig['user'],
            $this->mqConfig['password']
        );
        //创建通道
        $this->channel = $this->connection->channel();
    }

    /**
     * 发送消息
     * @param $data
     */
    public function send($data)
    {
        /**
         * 创建队列(Queue)
         * name: hello          队列名称
         * passive: false      如果设置true存在则返回OK，否则就报错。设置false存在返回OK，不存在则自动创建
         * durable: true       是否持久化，设置false是存放到内存中的，RabbitMQ重启后会丢失;设置true,则代表是一个持久化的队列，服务重启后也会存在，因为服务会把持久化的queue存放到磁盘上当服务重启的时候，会重新加载之前被持久化的queue
         * exclusive: false    是否排他，指定该选项为true则队列只对当前连接有效，连接断开后自动删除
         * auto_delete: false 是否自动删除，当最后一个消费者断开连接之后队列是否自动被删除
         */
        $this->channel->queue_declare($this->mqConfig['queue_name'], false, true, false, false);

        /**
         * 创建交换机(Exchange)
         * name: hello          交换机名称
         * type: direct         交换机类型，分别为direct/fanout/topic，参考另外文章的Exchange Type说明。
         * passive: false       如果设置true存在则返回OK，否则就报错。设置false存在返回OK，不存在则自动创建
         * durable: false       是否持久化，设置false是存放到内存中的，RabbitMQ重启后会丢失
         * auto_delete: false   是否自动删除，当最后一个消费者断开连接之后队列是否自动被删除
         */
        $this->channel->exchange_declare($this->mqConfig['exchange_name'], 'direct', false, true, false);

        // 绑定消息交换机和队列
        $this->channel->queue_bind($this->mqConfig['queue_name'], $this->mqConfig['exchange_name'],$this->mqConfig['route_key']);

        // 将要发送数据变为json字符串
        $messageBody = json_encode($data, JSON_UNESCAPED_UNICODE);

        /**
         * 创建AMQP消息类型
         * delivery_mode 消息是否持久化
         * AMQPMessage::DELIVERY_MODE_NON_PERSISTENT  不持久化
         * AMQPMessage::DELIVERY_MODE_PERSISTENT      持久化
         */
        $message = new AMQPMessage($messageBody, [
            'content_type' => 'text/plain',
            'delivery_mode' => AMQPMessage::DELIVERY_MODE_PERSISTENT
        ]);

        /**
         * 发送消息
         * msg: $message            AMQP消息内容
         * exchange: vckai_exchange 交换机名称
         * routing_key: hello       路由key
         */
        $this->channel->basic_publish($message, $this->mqConfig['exchange_name'], $this->mqConfig['route_key']);

        // 关闭连接
        $this->stop();
    }
    // 关闭连接
    public function stop()
    {
        $this->channel->close();
        $this->connection->close();
    }

}// class end