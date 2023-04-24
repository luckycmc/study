<?php

/**
 * Created by PhpStorm.
 * User: f
 * Date: 2022/11/13
 * Time: 10:03
 */
class Loader
{
    /**
     * 自动加载的方法
     * @param $class_name 对应的类名
     */
     public static function  autoload($class_name)
     {
            $filename = ROOT_PATH.'/app/controller/'.$class_name.'.php';
            self::includeFile($filename);
     }

    /**
     * 判断文件是否存在并且加载文件
     * @param $filename
     */
     public static function includeFile($filename)
     {
           if (file_exists($filename)){
               include_once $filename;
           }else{
               echo $filename.' not exists'.PHP_EOL;
           }
     }
} // class end