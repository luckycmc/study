<?php
/**
 * Created by PhpStorm.
 * User: f
 * Date: 2020/9/28
 * Time: 16:17
 */

namespace SwoStar\Commands;
use SwoStar\Foundation\Application;
use SwoStar\Server\Http\HttpServer;
use SwoStar\Server\WebSocket\WebSocketServer;

/**
 * 以命令行的方式启动框架
 * Class Command
 * @package SwoStar\Command
 */
class Command
{
     //启动框架
     public static function run()
     {
           //检查是否是cli模式
           self::checkCli();
           //检查是否被有swoole扩展
           self::checkExtension();
           //显示提示命令
           self::showUsageUI();
           //解析参数命令
           self::parseCommand();
     }
     //检查启动方式
     protected static function checkCli()
     {
          if(php_sapi_name() !=='cli'){

                echo "只能在 cli 摸下运行".PHP_EOL;
                exit;
          }
     }
     //检查对应的扩展
     protected static function checkExtension()
     {
           if(!extension_loaded('swoole')){
               echo "请安装 swoole 扩展".PHP_EOL;
               exit;
           }
     }
     //命令提示
     protected static function showUsageUI()
     {
         global $argc;
         if($argc <=1 || $argc > 3){
             echo PHP_EOL;
             echo "----------------------------------------".PHP_EOL;
             echo "|               Swoole                 |".PHP_EOL;
             echo "|--------------------------------------|".PHP_EOL;
             echo '|    USAGE: php swostar.php commond      |'.PHP_EOL;
             echo '|--------------------------------------|'.PHP_EOL;
             echo '|    1. start    以debug模式开启服务   |'.PHP_EOL;
             echo '|    2. start -d 以daemon模式开启服务  |'.PHP_EOL;
             echo '|    3. status   查看服务状态          |'.PHP_EOL;
             echo '|    4. reload   热加载                |'.PHP_EOL;
             echo '|    5. stop     关闭服务              |'.PHP_EOL;
             echo "----------------------------------------".PHP_EOL;
             echo PHP_EOL;
             exit;
         }

     }
     //解析命令启动服务
     protected static function parseCommand()
     {
         global $argv;

         $command = $argv[1];
         $option  = isset($argv[2]) ? $argv[2] : '';
         switch ($command)
         {
             case 'start':
                 //设置一守护进程的方式启动数据
                 if($option == '-d'){

                 }
                 self::workerStart();
                 break;

             case 'reload':
                 self::workerReolad();
                 break;
             case 'status':
                 self::workerStatus();
                 break;
             case 'stop':
                 self::workerStop();
                 break;

         }
     }
     //启动服务
     protected static function workerStart()
     {
         $server = null;
          //获取启动服务器的类型
          $ServerType = app('config')->get('server.server_type');

          switch ($ServerType)
          {
              case 'WebScoket':  // webscoket 服务器
                   $server = new WebSocketServer();
                   break;
              case 'Http':      // http 服务器
                   $server = new HttpServer();
                   break;
          }
          //启动服务
          $server->start();
     }
     //重启服务
     protected static function workerReolad()
     {

     }
     //服务的状态
    protected static function workerStatus()
    {

    }
     //服务关闭
    protected static function workerStop()
    {

    }
}// class end