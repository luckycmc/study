<?php
namespace app\controller;
/**
 * Created by PhpStorm.
 * User: f
 * Date: 2023/5/12
 * Time: 17:22
 */
class Login
{
    public function login()
    {
         return json_encode(['success'=>1,'msg'=>'ok']);
    }
}