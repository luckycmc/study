#include "study_coroutine.h"
#include <unordered_map>
using Study::PHPCoroutine;
using Study::Coroutine;
//无序字典user_yield_coros
static std::unordered_map<long, Coroutine*> user_yield_coros;
//声明宏定义
ZEND_BEGIN_ARG_INFO_EX(arginfo_study_coroutine_create,0,0,1)
   ZEND_ARG_CALLABLE_INFO(0,func,0)
ZEND_END_ARG_INFO()
//协成切换参数
ZEND_BEGIN_ARG_INFO_EX(arginfo_study_coroutine_void, 0, 0, 0)
ZEND_END_ARG_INFO()

// 声明函数方法
static PHP_METHOD(study_coroutine_util,create);

PHP_METHOD(study_coroutine_util,create)
{  
     // zend_fcall_info 就是用来接收我们创建协程的时候传递的那个函数
    zend_fcall_info fci = empty_fcall_info;
    zend_fcall_info_cache fcc = empty_fcall_info_cache;
    zval result;

    ZEND_PARSE_PARAMETERS_START(1, -1)
        Z_PARAM_FUNC(fci, fcc)
        Z_PARAM_VARIADIC('*', fci.params, fci.param_count)
    ZEND_PARSE_PARAMETERS_END_EX(RETURN_FALSE);
    
    fci.retval = &result;
    //执行对应的函数
    if(zend_call_function(&fci,&fcc) != SUCCESS){
        
         return;
    }
    //创建协成
    PHPCoroutine::create(&fcc, fci.param_count, fci.params);
}
//协成切换
PHP_METHOD(study_coroutine_util,yield) 
{
     //获取当协成
     Coroutine* co = Coroutine::get_current();
     //保存当前协成
     user_yield_coros[co->get_cid()] =co;
     co->yield();//切换协成
     RETURN_TRUE;
}
//方法注册
const zend_function_entry study_coroutine_util_methods [] = 
{   
    PHP_ME(study_coroutine_util,create,arginfo_study_coroutine_create,ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(study_coroutine_util,yield,arginfo_study_coroutine_void,ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
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