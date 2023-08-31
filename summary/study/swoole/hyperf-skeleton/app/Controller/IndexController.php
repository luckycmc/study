<?php

declare(strict_types=1);
/**
 * This file is part of Hyperf.
 *
 * @link     https://www.hyperf.io
 * @document https://hyperf.wiki
 * @contact  group@hyperf.io
 * @license  https://github.com/hyperf/hyperf/blob/master/LICENSE
 */
namespace App\Controller;
use Hyperf\HttpServer\Contract\RequestInterface;
use Hyperf\HttpServer\Annotation\AutoController;

#[AutoController]
class IndexController
{
    // Hyperf 会自动为此方法生成一个 /index/index 的路由，允许通过 GET 或 POST 方式请求
    public function index(RequestInterface $request)
    {
        // 从请求中获得 id 参数
        $id = $request->input('id', 1);
        // 转换 $id 为字符串格式并以 plain/text 的 Content-Type 返回 $id 的值给客户端
        return (string)$id;
    }
}
