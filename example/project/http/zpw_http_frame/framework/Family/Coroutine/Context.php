<?php
namespace Family\Coroutine;
/**
 * 协程的上下文
 */
class Context
{
     private $request;
     private $response;
    
     public function __construct(\Swoole\Http\Request $request, \Swoole\Http\Response $response)
     {
         $this->request = $request;
         $this->response = $response;
     }

      /**
     * @return \swoole_http_request
     */
    public function getRequest()
    {
        return $this->request;
    }

    /**
     * @return \swoole_http_response
     */
    public function getResponse()
    {
        return $this->response;
    }
      /**
     * @param $key
     * @param $val
     */
    public function set($key, $val)
    {
        $this->map[$key] = $val;
    }

    /**
     * @param $key
     * @return mixed|null
     */
    public function get($key)
    {
        if (isset($this->map[$key])) {
            return $this->map[$key];
        }

        return null;
    }
}