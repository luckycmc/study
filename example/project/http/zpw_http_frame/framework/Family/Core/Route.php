<?php
namespace Family\Core;

class Route
{   
    /**
     * 文件的加载分发
     */
    public static function  dispatch($path)
    {
         //默认访问 controller/start.php 的 index方法
         if (empty($path) || '/' == $path) {
            $controller = 'Index';
            $method = 'Index';
        } else {
            $maps = explode('/', $path);
            $controller = ucfirst($maps[1]);
            $method = isset($maps[2])?$maps[2]:'';
        }
        $controllerClass = "app\\controller\\{$controller}";
        $class = new $controllerClass;
        return $class->$method();
    }

}// class end