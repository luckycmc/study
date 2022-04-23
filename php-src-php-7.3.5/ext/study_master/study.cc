/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2016 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: codinghuang                                                             |
  +----------------------------------------------------------------------+
*/

#include "php_study.h"

ZEND_BEGIN_ARG_INFO_EX(arginfo_study_void, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_study_coroutine_create, 0, 0, 1)
    ZEND_ARG_CALLABLE_INFO(0, func, 0)
ZEND_END_ARG_INFO()
/***测试接口 start***/
#include <stdio.h>
#include <iostream>
#include <uv.h>

using namespace std;

uint64_t repeat = 0;

static void callback(uv_timer_t *handle)
{
    repeat = repeat + 1;
    cout << "repeat count:" << repeat << endl;
}

PHP_FUNCTION(study_timer_test)
{
        uv_loop_t *loop = uv_default_loop();
    uv_timer_t timer_req;
    uv_timer_init(loop, &timer_req);
    
    uv_timer_start(&timer_req, callback, 1000, 1000);
        uv_run(loop, UV_RUN_DEFAULT);
}
/***测试接口 end***/
/*************PHP 函数接口注册 start**************/
PHP_FUNCTION(study_coroutine_create);

PHP_FUNCTION(study_event_init)
{
    int ret;
    ret = st_event_init();
    if (ret < 0)
    {
        RETURN_FALSE;
    }
    RETURN_TRUE;
}

PHP_FUNCTION(study_event_wait)
{
    int ret;
    ret = st_event_wait();
    if (ret < 0)
    {
        RETURN_FALSE;
    }
    RETURN_TRUE;
}
/*************PHP 函数接口注册 start**************/
//php模块初始化 注册相关的入口函数
PHP_MINIT_FUNCTION(study)
{  
    study_coroutine_util_init();
    //协成服务器初始化
    study_coro_server_init(module_number);
    study_coro_channel_init();
     //socket 类的创建
    study_coro_socket_init(module_number);
    study_runtime_init();
    return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(study)
{
    return SUCCESS;
}

PHP_RINIT_FUNCTION(study)
{
    return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(study)
{
    return SUCCESS;
}

PHP_MINFO_FUNCTION(study)
{
    php_info_print_table_start();
    php_info_print_table_header(2, "study support", "enabled");
    php_info_print_table_end();
}
//php 函数和类注册 也就是提供给PHP 使用的接口
const zend_function_entry study_functions[] = {
    PHP_FE(study_coroutine_create, arginfo_study_coroutine_create)
    PHP_FALIAS(sgo, study_coroutine_create, arginfo_study_coroutine_create)
    PHP_FE(study_event_init, arginfo_study_void)
    PHP_FE(study_event_wait, arginfo_study_void)
    //增加测试代码
    PHP_FE(study_timer_test, NULL) // 新增加的一行
    PHP_FE_END
};
// study 模块注册
zend_module_entry study_module_entry = {
    STANDARD_MODULE_HEADER,
    "study",
    study_functions,
    PHP_MINIT(study),
    PHP_MSHUTDOWN(study),
    PHP_RINIT(study),
    PHP_RSHUTDOWN(study),
    PHP_MINFO(study),
    PHP_STUDY_VERSION,
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_STUDY
ZEND_GET_MODULE(study)
#endif
