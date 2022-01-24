<?php
/**
 * Created by PhpStorm.
 * User: f
 * Date: 2020/10/12
 * Time: 17:07
 */

namespace SwoStar\Core;
/**
 * trait 机制实现单例模式
 * Class Singleton
 * @package SwoStar\Core
 */
trait Singleton
{
    private static $instance;

    public static function getInstance(...$args)
    {
        if(!isset(self::$instance)){
            self::$instance = new static(...$args);
        }
        return self::$instance;
    }
}