#include "study_coroutine.h"
//声明宏定义
ZEND_BEGIN_ARG_INFO_EX(arginfo_study_coroutine_create,0,0,1)
   ZEND_ARG_CALLABLE_INFO(0,func,0)
ZEND_END_ARG_INFO()

// 声明函数方法
static PHP_METHOD(study_coroutine_util,create);

PHP_METHOD(study_coroutine_util,create)
{
    php_printf("success\n");
}

//方法注册
const zend_function_entry study_coroutine_util_methods [] = 
{   
    PHP_ME(study_coroutine_util,create,arginfo_study_coroutine_create,ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_FE_END
};


/**
 * Define zend class entry
 */
zend_class_entry study_coroutine_ce;
zend_class_entry *study_coroutine_ce_ptr;
//协成法法初始化
 void study_coroutine_util_init()
 {
     INIT_NS_CLASS_ENTRY(study_coroutine_ce, "Study", "Coroutine", study_coroutine_util_methods);
     study_coroutine_ce_ptr = zend_register_internal_class(&study_coroutine_ce TSRMLS_CC); // Registered in the Zend Engine
 }