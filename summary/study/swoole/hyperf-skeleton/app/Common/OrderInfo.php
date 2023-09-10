<?php
/**
 * Created by PhpStorm.
 * User: zpw
 * Date: 2023/9/6
 * Time: 22:34
 */

namespace App\Common;

/**
 * 订单的相关信息
 * Class OrderInfo
 * @package app\common
 */
class OrderInfo
{
    /**
     * 生成唯一订单号
     * @param string $prefix
     * @return string
     */
    public static function getOrderId($prefix = 'DD')
    {
        return $prefix . (strtotime(date('YmdHis', time()))) . substr(microtime(), 2, 6) . sprintf('%03d', rand(0, 999));
    }

    /**
     * 生成唯一订单号
     * @param string $user_id
     * @return string
     */
    public static function getOrderNo()
    {

        $houseNum = time()-strtotime(date('Y-m-d',time()));

        $time = date('ymd');

        $length = 20;

        $prefixTime = $time.$houseNum;

        $lastLen = $length - strlen($prefixTime);

        $utimestamp = microtime(true);

        $timestamp = floor($utimestamp);

        $milliseconds = round(($utimestamp - $timestamp) * 1000000);

        $orderNo = $prefixTime.$milliseconds;

        if($lastLen-strlen($milliseconds)>0){

            for($i=0;$i<($lastLen-strlen($milliseconds));$i++){

                $orderNo .= rand(1,9);

            }
        }
        return $orderNo;
    }
}