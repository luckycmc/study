<?php
namespace SwoCloud;

use Redis;
use SwoCloud\Supper\Arithmetic;
use Swoole\Http\Request;
use Swoole\Http\Response;
use Firebase\JWT\JWT;
use Swoole\Server as SwooleServer;

class Dispatcher
{
    //注册服务路由信息
    public function register(Route $route,SwooleServer $server, $fd, $data)
    {
        dd('register', 'Dispatcher');

        $serverKey = $route->getServerKey();
        // 把服务端的信息记录到redis中
        $redis = $route->getRedis();
        $value = \json_encode([
            'ip'   => $data['ip'],
            'port' => $data['port'],
        ]);
        $redis->sadd($serverKey, $value);
        // 这里是通过触发定时判断，不用heartbeat_check_interval 的方式检测
        // 是因为我们还需要主动清空，redis 数据
        //
        // $timer_id 定时器id
        $server->tick(3000, function($timer_id, Redis $redis,SwooleServer $server, $serverKey, $fd, $value){
            // 判断服务器是否正常运行，如果不是就主动清空
            // 并把信息从redis中移除
            if (!$server->exist($fd)) {
                $redis->srem($serverKey, $value);
                $server->clearTimer($timer_id);

                dd('im server 宕机， 主动清空');
            }
        }, $redis, $server, $serverKey, $fd, $value);
    }
    //用户登录
     public function login(Route $route,Request $request,Response $response)
     {
         // 获取im-server服务器
         /*
           192.168.186.130:9000
           192.168.186.130:9300
           192.168.186.128:9300
          */
         $imServer = \json_decode($this->getIMServer($route), true);
         dd($imServer, '这是获取的服务器信息');

         $url = $imServer['ip'].":".$imServer['port'];
         // 去数据库中进行用户密码和账号验证

         $uid = $request->post['id'];
         $token = $this->getToken($uid, $url);
         dd($token, "生成的token");
         $response->end(\json_encode(['token' => $token, 'url' => $url]));
     }
     //获取token 认证
    protected function getToken($uid, $url)
    {
        $key = "swocloud";
        $time = \time();
        $payload = array(
            "iss" => "http://example.org",
            "aud" => "http://example.com",
            "iat" => $time, // 签发时间
            "nbf" => $time,  // 生效时间
            "exp" => $time + (60 * 60 * 24),
            "data" => [
                "uid"       => $uid,
                "name"      => 'shineyork'.$uid,
                "serverUrl" => $url
            ],
        );
        return JWT::encode($payload, $key);
        // $decoded = JWT::decode($jwt, $key, array('HS256'));
    }
     //获取对应的服务器
     public function getIMServer(Route $route)
     {
         // 所有的服务信息
         $imServers = $route->getRedis()->smembers($route->getServerKey());
         //返回服务信息
         if (!empty($imServers)) {
             // 获取服务器的请求方式
             $serverS = $route->getArithmetic();
             return Arithmetic::$serverS($imServers);
        }
         return false;
     }
}// class end
