<?php
/**
 * Created by PhpStorm.
 * User: f
 * Date: 2020/10/10
 * Time: 14:57
 */

namespace SwoStar\Config;
/**
 * 获取对应的配置信息 功能
 * Class Config
 * @package SwoStar\Config
 */
class Config
{

    protected $itmes = [];
    protected $configPath = '';
    //构造方法
    function __construct()
    {
        //文件路径
        $this->configPath = app()->getBasePath().'/config';
        // 读取配置
        $this->itmes = $this->phpParser();
        //dd($this->itmes);
    }
    /**
     * 读取PHP文件类型的配置文件
     * @return [type] [description]
     */
    protected function phpParser()
    {
        // 1. 找到文件
        // 此处跳过多级的情况
        $files = scandir($this->configPath);  //获取执行目录下的配置文件  获取对应的配置信息
        $data = null;
        // 2. 读取文件信息
        foreach ($files as $key => $file)
        {
            if ($file === '.' || $file === '..') {
                continue;
            }
            //获取对应的PHP 文件
            $filename = stristr($file, ".php", true);
            //判断配置文件是否存在 存在加载对应配置信息
            $config_file = $this->configPath."/".$file;
            //配置文件存在则加载对应的配置文件
            if(file_exists($config_file)){

                $data[$filename] = include $config_file;

            }

        }
        //返回对应 的配置信息
        return $data;
    }
    // key.key2.key3
    // 获取对应的 配置键值信息
    public function get($keys)
    {
        $data = $this->itmes;
        foreach (\explode('.', $keys) as $key => $value)
        {
            $data = $data[$value];
        }
        return $data;
    }

}// class end