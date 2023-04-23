<?php

class Goods
{
    /**
     * 首页
     * @return array
     */
      public function index()
      {    
           $returnData = [
                'name'=>'zpw',
                'age'=>'23',
                'sex'=>1,
            ];
            return $returnData;
      }

    /**
     * 列表页
     * @return array
     */
      public function list()
      {
           // 这里只是简单的数据演示 事实上是可以操作数据库 mysql  和redis缓存
           /* return [
                'name'=>'zpw',
                'schoole'=>'许昌学院',
            ];*/
           $redis = new Redis();
           $redis->connect('127.0.0.1',6379);
           $data = $redis->get('rpc'); //获取redis中的数据
          return [
              'name'=>'zpw',
              'schoole'=>$data,
          ];
      }
}