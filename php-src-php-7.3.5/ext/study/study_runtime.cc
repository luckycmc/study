#include "study_runtime.h"
//参数的定义
ZEND_BEGIN_ARG_INFO_EX(arginfo_study_runtime_void,0,0,0)
ZEND_END_ARG_INFO()
/**
 * Define zend class entry
 */
zend_class_entry study_runtime_ce;
zend_class_entry *study_runtime_ce_ptr;

extern PHP_METHOD(study_coroutine_util, sleep);
static void hook_func(const char *name, size_t name_len, zif_handler handler);
//对应的接口
static PHP_METHOD(study_runtime, enableCoroutine)
{   //ZEND_STRL 会生成 sleep  和他的长度
    hook_func(ZEND_STRL("sleep"), zim_study_coroutine_util_sleep);
}

//实现hook_func  替换要查找的 函数
static void hook_func(const char *name, size_t name_len, zif_handler handler)
{    
    zend_function *ori_f = (zend_function *) zend_hash_str_find_ptr(EG(function_table), name, name_len);
    ori_f->internal_function.handler = handler;//指向协成后的指针函数 实现无缝衔接的切换
}
//注册类方法
static const zend_function_entry study_runtime_methods[] =
{
    PHP_ME(study_runtime, enableCoroutine, arginfo_study_runtime_void, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_FE_END
};


//服务器初始化
void study_runtime_init()
{
    INIT_NS_CLASS_ENTRY(study_runtime_ce, "Study", "Runtime", study_runtime_methods);
    study_runtime_ce_ptr = zend_register_internal_class(&study_runtime_ce TSRMLS_CC); // Registered in the Zend Engine
}