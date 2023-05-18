<?php
//创建一个http server服务
$server = new swoole_http_server('0.0.0.0', 8888);
 
$server->set([
     'worker_num' => swoole_cpu_num() * 2,
     
]);
 
$server->on('WorkerStart', function ($server, $worker_id) {
  //注册自动加载函数
  spl_autoload_register(function ($class) use($server) {
    $class = $server->setting['project_path'] . str_replace('\\', DIRECTORY_SEPARATOR, $class) . '.php';
 
    if (file_exists($class)) {
      include_once $class;
    }
  });
});
 
$server->on('request', function ($request, $response) use ($server) {
  $pathInfo = explode('/', ltrim($request->server['path_info'], '/'));
 
  //模块/控制器/方法
  $module = $pathInfo[0] ?? 'Index';
  $controller = $pathInfo[1] ?? 'Index';
  $method = $pathInfo[2] ?? 'index';
 
  try {
    $class = "\\{$module}\\{$controller}";
    $result = (new $class)->{$method}();
    $response->end($result);
  } catch (\Throwable $e) {
    $response->end($e->getMessage());
  }
});
 
//启动服务
$server->start();