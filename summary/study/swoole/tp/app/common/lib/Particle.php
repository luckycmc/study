<?php
/**
 * Created by PhpStorm.
 * User: f
 * Date: 2023/5/23
 * Time: 15:43
 */

namespace app\common\lib;

/**
 * 雪花算法生成唯一ID
 * Class Particle
 * @package app\common\lib
 */
class Particle
{
    const EPOCH = 1479533469598;
    const max12bit = 4095;
    const max41bit = 1099511627775;

    static $machineId = null;
    /**
     * //变量不会被销毁 会一直存放在 进程的栈上 只要变量不一直变大就不会
     * 发生内存泄露
     * @var int
     */
    public static $i = 0;
    public static function machineId($mId = 0) {
        self::$machineId = $mId;
    }

    public static function generateParticle() {
        /*
        * Time - 42 bits
        */
        $time = floor(microtime(true) * 1000);

        /*
        * Substract custom epoch from current time
        */
        $time -= self::EPOCH;

        /*
        * Create a base and add time to it
        */
        $base = decbin(self::max41bit + $time);

        /*
        * Configured machine id - 10 bits - up to 1024 machines
        */
        if(!self::$machineId) {
            $machineid = self::$machineId;
        } else {
            $machineid = str_pad(decbin(self::$machineId), 10, "0", STR_PAD_LEFT);
        }

        /*
        * sequence number - 12 bits - up to 4096 random numbers per machine
        */
        $random = str_pad(decbin(mt_rand(0, self::max12bit)), 12, "0", STR_PAD_LEFT);

        /*
        * Pack
        */
        $base = $base.$machineid.$random;

        /*
        * Return unique time id no
        */
        return bindec($base);
    }

    public static function timeFromParticle($particle) {
        /*
        * Return time
        */
        return bindec(substr(decbin($particle),0,41)) - self::max41bit + self::EPOCH;
    }

}// class end