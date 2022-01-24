<?php
use SwoStar\Routes\Route;

/**
 *
 * 闭包路由形式
 */
Route::get('index',function (){
    return "this is route index() tests";
});
//注册对应的路由
Route::get('/index/dd','IndexController@dd');
Route::get('/index/list','IndexController@list');
Route::get('/index/get','IndexController@get');
//对应的admin控制器
Route::get('/admin/get','AdminController@get');