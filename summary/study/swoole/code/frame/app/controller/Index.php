<?php

class Index
{
   
      public function index()
      {
           $title = 'swoole 留言板';
           $filename = ROOT_PATH.'/app/view/index/index.php';
           $content = file_get_contents($filename);
           return $content;
      }

      public function get()
      {
          return "get www.php.cn data";
      }

}// class end