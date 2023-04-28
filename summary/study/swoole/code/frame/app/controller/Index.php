<?php

class Index
{
    /**
     * 获取列表页数据
     * Index constructor.
     */
      public function index()
      {

           $conn = MysqlPool::getInstance()->getConn();
           $content = $conn->query('SELECT * FROM messages');
          //使用完回收连接 不然连接不能复用
           MysqlPool::getInstance()->recycle($conn);
           return json_encode($content);
      }

    /**
     * 获取单独的数据
     * @return string
     */
      public function get()
      {
          $id = $_GET['id']; //获取对应的id
          $conn = MysqlPool::getInstance()->getConn();
          $result = $conn->query('SELECT * FROM messages WHERE id='.$id);
          //使用完回收连接 不然连接不能复用
          MysqlPool::getInstance()->recycle($conn);
          return json_encode($result);
      }

     /**
      * 保存数据
      */
      public function save()
      {
          $test_title = "小张";
          $test_author = date('Y-m-d H:i:s',time());
          $submission_date = $_POST['content'];
          $sql = "INSERT INTO messages ".
                  "(`name`,`time`,`content`) ".
                  "VALUES ".
                  "('$test_title','$test_author','$submission_date')";
          print_r($sql);
          $conn = MysqlPool::getInstance()->getConn();
          $result = $conn->query($sql);
          //使用完回收连接 不然连接不能复用
          MysqlPool::getInstance()->recycle($conn);
          return $result ? json_encode(['status'=>1,'msg'=>'success']) : json_encode(['status'=>0,'msg'=>'faild']);
      }
}// class end