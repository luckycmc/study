<?php
namespace SwoStar\Console;

class Input
{
    /**
     * 打印输出对应的信息
     * @param $message
     * @param null $description
     */
    public static function info($message, $description = null)
    {
        $return = "======>>> ".$description." start\n";
        if (\is_array($message)) {

            //函数用于输出或返回一个变量，以字符串形式表示
            $return = $return.\var_export($message, true);
        } else {
            $return .= $message."\n";
        }
        $return .= "======>>> ".$description." end\n";
        echo $return;
    }
}
