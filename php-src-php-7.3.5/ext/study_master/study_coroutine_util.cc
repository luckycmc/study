#include "study_coroutine.h"
#include <unordered_map>

using study::PHPCoroutine;
using study::Coroutine;
//把获取的当前协程存入一个无序字典user_yield_coros里面。
static std::unordered_map<long, Coroutine *> user_yield_coros;

ZEND_BEGIN_ARG_INFO_EX(arginfo_study_coroutine_void, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_study_coroutine_resume, 0, 0, 1)
    ZEND_ARG_INFO(0, cid)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_study_coroutine_isExist, 0, 0, 1)
    ZEND_ARG_INFO(0, cid)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_study_coroutine_create, 0, 0, 1)
    ZEND_ARG_CALLABLE_INFO(0, func, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_study_coroutine_defer, 0, 0, 1)
    ZEND_ARG_CALLABLE_INFO(0, func, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_study_coroutine_sleep, 0, 0, 1)
    ZEND_ARG_INFO(0, seconds)
ZEND_END_ARG_INFO()

PHP_FUNCTION(study_coroutine_create)
{
    zend_fcall_info fci = empty_fcall_info;
    zend_fcall_info_cache fcc = empty_fcall_info_cache;

    ZEND_PARSE_PARAMETERS_START(1, -1)
        Z_PARAM_FUNC(fci, fcc)
        Z_PARAM_VARIADIC('*', fci.params, fci.param_count)
    ZEND_PARSE_PARAMETERS_END_EX(RETURN_FALSE);

    long cid = PHPCoroutine::create(&fcc, fci.param_count, fci.params);
    RETURN_LONG(cid);
}
/**
   当前协成让出
 */
PHP_METHOD(study_coroutine_util,yield)
{
     //获取当前协成的堆栈信息
     Coroutine* co = Coroutine::get_current();
     user_yield_coros[co->get_cid()] = co; //保存当前协成的的信息
     co->yield(); //让出当前协成
     RETURN_TRUE;
}
/**

  恢复当前协成
 * 
 */
PHP_METHOD(study_coroutine_util, resume)
{
    zend_long cid = 0;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_LONG(cid)
    ZEND_PARSE_PARAMETERS_END_EX(RETURN_FALSE);

    auto coroutine_iterator = user_yield_coros.find(cid); //自动查找协成的堆栈
    if (coroutine_iterator == user_yield_coros.end())  //找不到
    {
        php_error_docref(NULL, E_WARNING, "resume error"); 
        RETURN_FALSE;
    }

    Coroutine* co = coroutine_iterator->second;
    user_yield_coros.erase(cid); //清除当前的信息
    co->resume();
    RETURN_TRUE;
}
/**
 获取当期协成的id
 * 
 */
PHP_METHOD(study_coroutine_util, getCid)
{
    Coroutine* co = Coroutine::get_current(); //获取当前的协程的信息
    if (co == nullptr)
    {
        RETURN_LONG(-1);
    }
    RETURN_LONG(co->get_cid());// 当前协成对应的id
}
/**
 当前的协程是否存在
 * 
 */
PHP_METHOD(study_coroutine_util, isExist)
{
    zend_long cid = 0;
    bool is_exist;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_LONG(cid)
    ZEND_PARSE_PARAMETERS_END_EX(RETURN_FALSE);

    auto coroutine_iterator = Coroutine::coroutines.find(cid);
    is_exist = (coroutine_iterator != Coroutine::coroutines.end());
    RETURN_BOOL(is_exist);
}

PHP_METHOD(study_coroutine_util, defer)
{
    zend_fcall_info fci = empty_fcall_info;
    zend_fcall_info_cache fcc = empty_fcall_info_cache;
    php_study_fci_fcc *defer_fci_fcc;

    defer_fci_fcc = (php_study_fci_fcc *)emalloc(sizeof(php_study_fci_fcc));

    ZEND_PARSE_PARAMETERS_START(1, -1)
        Z_PARAM_FUNC(fci, fcc)
    ZEND_PARSE_PARAMETERS_END_EX(RETURN_FALSE);

    defer_fci_fcc->fci = fci;
    defer_fci_fcc->fcc = fcc;

    PHPCoroutine::defer(defer_fci_fcc);
}
/**
 * 模拟IO阻塞
 * 
 */
PHP_METHOD(study_coroutine_util, sleep)
{
    double seconds;
    
    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_DOUBLE(seconds)
    ZEND_PARSE_PARAMETERS_END_EX(RETURN_FALSE);

    if (UNEXPECTED(seconds < 0.001))
    {
        php_error_docref(NULL, E_WARNING, "Timer must be greater than or equal to 0.001");
        RETURN_FALSE;
    }

    PHPCoroutine::sleep(seconds);
    RETURN_TRUE;
}
//当前类对应的操作方法
static const zend_function_entry study_coroutine_util_methods[] =
{   
    ZEND_FENTRY(create, ZEND_FN(study_coroutine_create), arginfo_study_coroutine_create, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    //注册yield 方法
    PHP_ME(study_coroutine_util, yield, arginfo_study_coroutine_void, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    //注册 resume 方法
    PHP_ME(study_coroutine_util, resume, arginfo_study_coroutine_resume, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    //注册getCid()获取当前的属性
    PHP_ME(study_coroutine_util, getCid, arginfo_study_coroutine_void, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(study_coroutine_util, isExist, arginfo_study_coroutine_isExist, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(study_coroutine_util, defer, arginfo_study_coroutine_defer, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(study_coroutine_util, sleep, arginfo_study_coroutine_sleep, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_FE_END
};

/**
 * 
 *Study\Coroutine 类的注册
 * Define zend class entry
 */
zend_class_entry study_coroutine_ce;
zend_class_entry *study_coroutine_ce_ptr;

void study_coroutine_util_init()
{
    PHPCoroutine::init();
    INIT_NS_CLASS_ENTRY(study_coroutine_ce, "Study", "Coroutine", study_coroutine_util_methods);
    study_coroutine_ce_ptr = zend_register_internal_class(&study_coroutine_ce TSRMLS_CC); // Registered in the Zend Engine
    zend_register_class_alias("SCo", study_coroutine_ce_ptr);
}