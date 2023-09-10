<?php
declare(strict_types=1);
/**
 * Created by PhpStorm.
 * User: zpw
 * Date: 2023/9/10
 * Time: 22:58
 */

namespace App\Process;


use App\Model\Goods;
use Hyperf\Process\AbstractProcess;
use Hyperf\Process\Annotation\Process;
use Swoole\Timer;

#[Process(name: "user-process", redirectStdinStdout: false, pipeType: 2, enableCoroutine: true)]
class FooProcess extends AbstractProcess
{
     public int $nums = 2;
    public function handle(): void
    {
         while (true){
             go(function (){
                 $this->dealData();
             });

         }
    }
    public function dealData(){
        //查询商品
        $goods = Goods::find(1);
        var_dump($goods);
    }
}