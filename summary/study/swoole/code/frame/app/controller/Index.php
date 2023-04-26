<?php

class Index
{
    /**
     * 获取列表页数据
     * Index constructor.
     */
      public function index()
      {
           $content = [
                ['name'=>"小张",'time'=> "2020-01-01",'content'=> "今天星期五，明天星期六！" ],
                 ['name'=> "小磊", 'time'=> "2020-02-01",'content'=> "中秋节快乐！" ],
                 ['name'=>"小海", 'time'=> "2020-03-01", 'content'=> "明天不上课！爽翻！！！" ],
                 ['name'=> "小丽", 'time'=> "2020-04-01", 'content'=> "今晚做个好梦哦~" ],
           ];
           return json_encode($content);
      }

    /**
     * 获取单独的数据
     * @return string
     */
      public function get()
      {
          $conn = MysqlPool::getInstance()->getConn();
          $result = $conn->query('SELECT * FROM messages WHERE id=1');
          return json_encode($result);
      }

    /**
     * 保存数据
     */
      public function save()
      {
          $test_title = "小张";
          $test_author = date('Y-m-d H:i:s',time());
          $submission_date = '今天星期五，明天星期六！';
          $sql = "INSERT INTO messages ".
                  "(`name`,`time`,`content`) ".
                  "VALUES ".
                  "('$test_title','$test_author','$submission_date')";
          print_r($sql);
          $conn = MysqlPool::getInstance()->getConn();
          $result = $conn->query($sql);
          var_dump($result);
      }
}// class end