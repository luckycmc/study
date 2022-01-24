<?php
/**
 * Created by PhpStorm.
 * User: f
 * Date: 2020/10/30
 * Time: 15:41
 */

namespace SwoStar\Model;


use SwoStar\Core\Singleton;

class Db
{
     use Singleton;

     protected $db = null;

     public function set($obj = null)
     {
          $this->db = $obj;
     }
     public function get()
     {
         return $this->db;
     }
}// class end