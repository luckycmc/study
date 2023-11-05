<?php
namespace Family;

use Family\Core\Config;
use Family\Core\Route;
use Family\Coroutine\Context;
use Family\Coroutine\Coroutine;
use Family\Pool\Context as PoolContext;

class Family
{    
    
    /**
    * @var 根目录
     */
    public static $rootPath;
    /**
     * @var 框架目录
     */
    public static $frameworkPath;
    /**
     * @var 程序目录
     */
    public static $applicationPath;
    /**
     * 启动服务
     */
     final public static function run()
     {  
         if (!defined('DS')) {
            define('DS', DIRECTORY_SEPARATOR);
         }
         /****定义相应的目录 start***/
         self::$rootPath = dirname(dirname(__DIR__));
         self::$frameworkPath = self::$rootPath . DS . 'framework';
         self::$applicationPath = self::$rootPath . DS . 'application';    
         /****定义相应的目录 end***/
          //1.注册自动加载函数
          \spl_autoload_register(__CLASS__.'::autoLoader');
          //2.获取配置文件
          Config::load();
          $http = new \Swoole\Http\Server(Config::get('host'), Config::get('port'));
          $http->set([
            "worker_num" =>Config::get('worker_num'),
          ]);
          $http->on('request',function(\Swoole\Http\Request $request, \Swoole\Http\Response $response){
            //初始化根协程ID
            $coId = Coroutine::setBaseId();
            //初始化上下文
            $context = new Context($request,$response);
             //存放容器pool
            PoolContext::set($context);
             //协程退出，自动清空
            defer(function() use($coId){
                 PoolContext::clear($coId);
             });
            try{
                 //启动路由加载：
                $result = Route::dispatch($request->server['path_info']);
                 //响应数据
                 $response->end($result);
            }catch(\Exception $e){
                $response->end($e->getMessage());
            }catch(\Error $e){
                $response->end($e->getMessage());  
            }
           
          });
          //启动服务
          $http->start();
     }
     /**
 * @param $class
 * @desc 自动加载类
 */
final public static function autoLoader($class)
{
        //定义rootPath
        $rootPath = dirname(dirname(__DIR__));

        //把类转为目录，eg \a\b\c => /a/b/c.php
        $classPath = \str_replace('\\', DIRECTORY_SEPARATOR, $class) . '.php';

        //约定框架类都在framework目录下, 业务类都在application下
        $findPath = [
            $rootPath . DIRECTORY_SEPARATOR . 'framework' . DIRECTORY_SEPARATOR,
            $rootPath . DIRECTORY_SEPARATOR . 'application' . DIRECTORY_SEPARATOR,
        ];

        //遍历目录，查找文件
        foreach ($findPath as $path) {
            //如果找到文件，则require进来
            $realPath = $path . $classPath;
            if (is_file($realPath)) {
                require "{$realPath}";
                return;
            }
        }

   }

}// class end

