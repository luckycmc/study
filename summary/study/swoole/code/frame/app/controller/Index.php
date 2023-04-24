<?php

class Index
{
   
      public function index()
      {
           $content = [
                'title'=>'swoole 留言板'
           ];
           return json_encode($content);
      }

      public function get()
      {
          $conn = MysqlPool::getInstance()->getConn();
          $conn->query('SELECT * FROM fa_admin WHERE id=1');
          return "get www.php.cn data";
      }
      //获取列表

}// class end