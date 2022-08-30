#include "php_study.h"

ZEND_BEGIN_ARG_INFO_EX(arginfo_study_coroutine_create, 0, 0, 1)
    ZEND_ARG_CALLABLE_INFO(0, func, 0)
ZEND_END_ARG_INFO()
//事件参数
ZEND_BEGIN_ARG_INFO_EX(arginfo_study_coroutine_void, 0, 0, 0)
ZEND_END_ARG_INFO()

/***测试接口 start***/
#include <stdio.h>
// swoole 为什么有两个栈切换 handler 执行的是c函数
/***测试接口 end***/
PHP_FUNCTION(study_coroutine_create);

PHP_MINIT_FUNCTION(study)
{   
    study_coroutine_util_init();
    //服务器初始化
    //study_coroutine_server_coro_init(); // 新增加的代码
    study_coro_server_init(module_number); // 修改的地方
    //channel
    study_coro_channel_init(); // 新增的一行
    study_coro_socket_init(module_number); // 新增的一行
    //hook类初始化
    study_runtime_init(); // 新增的代码
    return SUCCESS;
}
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

//  src/coroutine/coroutine.cc 代码可能有问题导致编译不能成功
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

const zend_function_entry study_functions[] = {
    //注册替换函数 替换对应的类
    PHP_FE(study_coroutine_create, arginfo_study_coroutine_create)
    PHP_FALIAS(sgo, study_coroutine_create, arginfo_study_coroutine_create)
    PHP_FE(study_event_init, arginfo_study_coroutine_void)
    PHP_FE(study_event_wait, arginfo_study_coroutine_void)
    PHP_FE_END
};

zend_module_entry study_module_entry = {
    STANDARD_MODULE_HEADER,
    "study",
    study_functions,    //注册模块函数
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
