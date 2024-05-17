<?php
/**
 * @param $a
 * @return mixed
 */
if(!function_exists('app')){

    function app($a = null)
    {
        if(empty($a)){

            return \SwoStar\Foundation\Application::getInstance();
        }

        return \SwoStar\Foundation\Application::getInstance()->make($a);
    }
}
if(!function_exists('dd')){
    /**
     * 添加助手打印函数
     */
        function dd($message,$description = null)
        {
            \SwoStar\Console\Input::info($message,$description);
        }
}