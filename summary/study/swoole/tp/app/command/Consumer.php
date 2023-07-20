<?php
declare (strict_types = 1);

namespace app\command;

use think\console\Command;
use think\console\Input;
use think\console\input\Argument;
use think\console\input\Option;
use think\console\Output;

class Consumer extends Command
{
    protected function configure()
    {
        // 指令配置
        $this->setName('app\command\consumer')
            ->setDescription('the app\command\consumer command');
    }

    protected function execute(Input $input, Output $output)
    {
        $consumer = new \app\common\service\rabbitmq\Consumer();
        $consumer->start();
        // 指令输出
        $output->writeln('app\command\consumer');
    }
}
