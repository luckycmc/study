<?php
    $serv = new tinyswoole_server('127.0.0.1', 9501);
    $serv->set([
          'worker_num'=>6,
          'reactor_num'=>2
    ]);
    $serv->start();