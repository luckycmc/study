<?php
namespace SwoCloud;
use Swoole\Process;

/**
 * 服务启动框架
 * Class SwoCloud
 * @package SwoCloud
 */
class SwoCloud
{
    //之前的启动
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
                //实例化服务
                 $routeServer = (new Route);
                //设置一守护进程的方式启动数据
                if($option == '-d'){
                    //设置守护进程的方式启动
                    $routeServer->setConfig([
                        'daemonize' => 1
                    ]);
                }
                self::workerStart($routeServer);
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
    protected static function workerStart(Route $routeServer)
    {
        //启动服务
        $routeServer->start();
    }
    //重启服务
    protected static function workerReolad()
    {
          $master_pid = file_get_contents(SWOSTAR_ROOT.'/runtime/master_pid.pid');
         //给主进程发信号重启进程服务
          Process::kill($master_pid,SIGUSR1);
          //获取对应的进程id文件
          $worker_pid = SWOSTAR_ROOT.'/runtime/worker_pid.pid';
          //热重时 删除对应的记录id
          if(file_exists($worker_pid)){
               unlink($worker_pid);
          }
           //如果存在task 进程id 则直接删除
          $task_pid = SWOSTAR_ROOT.'/runtime/task_pid.pid';
          if(file_exists($task_pid)){

                unlink($task_pid); //删除异步任务pid
          }
          echo date('Y-m-d H:i:s',time()).":热加载成功....".PHP_EOL;
          return true;
    }
    //服务的状态
    protected static function workerStatus()
    {
        //主进程pid
        $master_pid  = SWOSTAR_ROOT.'/runtime/master_pid.pid';
        $manager_pid = SWOSTAR_ROOT.'/runtime/manager_pid.pid';
        //获取对应的进程id文件
        $worker_pid = SWOSTAR_ROOT.'/runtime/worker_pid.pid';
        //判断进程文件是否存在
        if(!file_exists($master_pid) || !file_exists($manager_pid) || !file_exists($worker_pid)){

                 echo "暂无启动的服务";
                 return false;
        }
        //显示进程信息
        self::showProcessUI();
        // 获取进程信息
        $master_pid_pid = file_get_contents($master_pid);
        $manager_pid_pid = file_get_contents($manager_pid);


        echo str_pad("Master",30,' ', STR_PAD_BOTH).
             str_pad($master_pid_pid,26,' ',STR_PAD_BOTH).
             str_pad($manager_pid_pid, 36,' ', STR_PAD_BOTH).PHP_EOL;
    }
    //服务关闭
    protected static function workerStop()
    {
        //主进程pid
        $master_pid  = SWOSTAR_ROOT.'/runtime/master_pid.pid';
        if(!file_exists($master_pid)){

             echo "暂时无服务启动...".PHP_EOL;
        }
        //获取主进程id
        $master_pid_string = file_get_contents($master_pid);
        Process::kill($master_pid_string); //杀死进程
        $time = time();
        while (true){
            usleep(2000);
            if(!Process::kill($master_pid_string,0)){
                //主进程pid
                $master_pid  = SWOSTAR_ROOT.'/runtime/master_pid.pid';
                $manager_pid = SWOSTAR_ROOT.'/runtime/manager_pid.pid';
                //获取对应的进程id文件
                $worker_pid = SWOSTAR_ROOT.'/runtime/worker_pid.pid';
                /***********************删除对应的pid文件 start**********************/
                if(file_exists($master_pid)){
                     unlink($master_pid);
                }
                if(file_exists($manager_pid)){
                    unlink($manager_pid);
                }
                if(file_exists($worker_pid)){
                    unlink($worker_pid);
                }
                /***********************删除对应的pid文件 end**********************/
                break;
            }else{

                  //超过5秒服务器要重试
                  if(time() - $time){

                         echo "服务器关闭失败,请重试....".PHP_EOL;
                         break;
                  }
            }
        }
        echo "服务器关闭成功....".PHP_EOL;
    }
    //显示数据界面
    protected static function showProcessUI()
    {
          echo str_pad("-",90,'-',STR_PAD_BOTH).PHP_EOL;
          echo "|".str_pad("启动/关闭", 92, ' ', STR_PAD_BOTH) . "|" . PHP_EOL;
          echo str_pad("-",90,'-',STR_PAD_BOTH).PHP_EOL;
          echo str_pad("Start success.", 50, ' ', STR_PAD_BOTH).
                           str_pad("php bin/swocloud stop", 50, ' ', STR_PAD_BOTH) . PHP_EOL;
          echo str_pad("-",90,'-',STR_PAD_BOTH).PHP_EOL;
          echo "|" .str_pad("版本信息",92, ' ',STR_PAD_BOTH) ."|".PHP_EOL;
          echo str_pad("-",90,'-',STR_PAD_BOTH).PHP_EOL;
          echo str_pad("Swoole Version:" .SWOOLE_VERSION, 50, ' ',STR_PAD_BOTH) .
                        str_pad("PHP Version:".PHP_VERSION,50,' ',STR_PAD_BOTH).PHP_EOL;
          echo str_pad("-",90,'-',STR_PAD_BOTH).PHP_EOL;
          //ip信息
          echo "|" .str_pad("IP 信息", 90,' ',STR_PAD_BOTH) ."|" .PHP_EOL;
          echo str_pad("-",90,'-',STR_PAD_BOTH).PHP_EOL;
          echo str_pad("IP:192.168.32.129" , 50,' ',STR_PAD_BOTH).str_pad("PORT:9500" , 50, ' ',STR_PAD_BOTH).PHP_EOL;
          echo str_pad("-",90,'-',STR_PAD_BOTH).PHP_EOL;
          //进程信息
          echo "|".str_pad("进程信息",92,' ',STR_PAD_BOTH)."|" .PHP_EOL;
          echo str_pad("-",90,'-',STR_PAD_BOTH).PHP_EOL;
          echo str_pad("Swoole进程",30,' ',STR_PAD_BOTH).
                    str_pad('master_pid',30,' ',STR_PAD_BOTH).
                    str_pad('manager_pid',30,' ',STR_PAD_BOTH) .PHP_EOL;
    }
}//class end
