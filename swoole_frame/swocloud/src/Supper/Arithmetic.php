<?php
namespace SwoCloud\Supper;
/**
 * yo
 */
class Arithmetic
{
    protected static $roundLastIndex = 0;
    //轮训的方式
    public static function round(array $list)
    {
        $index = self::$roundLastIndex;
        $url = $list[$index];
        if ($index + 1 > count($list) - 1) {
            self::$roundLastIndex = 0;
        } else {
            self::$roundLastIndex++;
        }
        return  $url;
    }
    // hash 的方式
    public static function hash()
    {

    }
    //权重的方式
    public static function weight()
    {

    }
}// class end
