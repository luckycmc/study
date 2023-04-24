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
          return "get www.php.cn data";
      }
      //获取列表

}// class end