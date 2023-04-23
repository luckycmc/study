<?php

class Index
{
   
      public function index()
      {
           $content = [
                'title'=>'swoole 留言板'
           ];
           return $content;
      }

      public function get()
      {
          return "get www.php.cn data";
      }

}// class end