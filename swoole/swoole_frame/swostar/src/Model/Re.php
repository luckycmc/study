<?php
/**
 * Created by PhpStorm.
 * User: f
 * Date: 2020/10/30
 * Time: 15:52
 */

namespace SwoStar\Model;


use SwoStar\Core\Singleton;

class Re
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

}