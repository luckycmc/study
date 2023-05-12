<?php
/**
 * Created by PhpStorm.
 * User: zpw
 * Date: 2023/5/12
 * Time: 21:50
 */

namespace app\controller;


use app\BaseController;

/**
 * 基类控制器
 * Class Base
 * @package app\controller
 */
class Base extends BaseController
{
     public function __construct(App $app)
     {
         parent::__construct($app);
     }
}