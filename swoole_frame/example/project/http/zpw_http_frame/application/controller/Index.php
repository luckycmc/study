<?php
namespace app\controller;

/**
 * Class Index
 * @package controller
 */
class Index extends Base
{
    public function index()
    {
          $data = $this->request->get;
          return 'i am family by route!' . json_encode($data);
    }

    public function zpw()
    {
        return 'i am tong ge zpw';
    }
}