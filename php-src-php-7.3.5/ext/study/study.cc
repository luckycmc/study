#include "php_study.h"

ZEND_BEGIN_ARG_INFO_EX(arginfo_study_coroutine_create, 0, 0, 1)
    ZEND_ARG_CALLABLE_INFO(0, func, 0)
ZEND_END_ARG_INFO()
/***测试接口 start***/
#include <stdio.h>

/***测试接口 end***/
PHP_FUNCTION(study_coroutine_create);

PHP_MINIT_FUNCTION(study)
{   
    study_coroutine_util_init();
    return SUCCESS;
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
