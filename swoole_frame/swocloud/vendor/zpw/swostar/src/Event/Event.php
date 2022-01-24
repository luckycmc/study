<?php
/**
 * Created by PhpStorm.
 * User: zpw
 * Date: 2020/10/18
 * Time: 21:47
 */

namespace SwoStar\Event;


/**
 *
 */
class Event
{
    protected $events = [];

    /**
     * 事件注册
     * @param  string $event    事件标识
     * @param  Closure $callback 事件回调函数 也是闭包函数
     */
    public function register($event, $callback)
    {
        $event = \strtolower($event);

        // 判断事件是否存在
        // if (condition) {
        //   // code...
        // }

        $this->events[$event] = ['callback' => $callback];
    }
    /**
     * 事件的触发函数
     * @param  string $event 事件标识
     * @param  array  $param 事件参数
     */
    public function trigger($event, $param = [])
    {
        $event = \strtolower($event);
         var_dump($event);
        if (isset($this->events[$event])) {
            ($this->events[$event]['callback'])(...$param);
            dd('事件执行成功');
            return true;
        }
        dd('事件不存在');
    }

    /**
     * 获取相应的事件
     * @param null $event
     * @return array|mixed
     */
    public function getEvents($event = null)
    {
        return empty($event) ? $this->events : $this->events[$event];
    }

}// class end
