<?php
namespace app\controller;

use app\BaseController;
use think\facade\Db;


class Index extends Base
{
    public function index()
    {
        $goods_id = rand(1,25166);
        $data = Db::name('lmrs_products')
            ->field('name,sold_count')
            ->where(['id'=>$goods_id])
            ->find();
        return '<style type="text/css">*{ padding: 0; margin: 0; } div{ padding: 4px 48px;} a{color:#2E5CD5;cursor: pointer;text-decoration: none} a:hover{text-decoration:underline; } body{ background: #fff; font-family: "Century Gothic","Microsoft yahei"; color: #333;font-size:18px;} h1{ font-size: 100px; font-weight: normal; margin-bottom: 12px; } p{ line-height: 1.6em; font-size: 42px }</style><div style="padding: 24px 48px;"> <h1>:) </h1><p> ThinkPHP V' . \think\facade\App::version() . '<br/><span style="font-size:30px;">16载初心不改 - 你值得信赖的PHP框架</span></p><span style="font-size:25px;">[ V6.0 版本由 <a href="https://www.yisu.com/" target="yisu">亿速云</a> '.$this->web_title.' ]</span></div><script type="text/javascript" src="https://e.topthink.com/Public/static/client.js"></script><think id="ee9b1aa918103c4fc"></think>';
    }

    public function hello()
    {
        return 'hello,';
    }

    /**
     * 修改测试数据
     * @return string
     */
    public function test()
    {
        //先从redis获取 没有则在从 数据库中获取

        $data = Db::name('lmrs_products')
                ->field('name,sold_count')
                ->order('sold_count desc')
                ->limit(10)
                ->select();

        return json_encode($data);
    }
}
