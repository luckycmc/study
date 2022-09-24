#include "study_coroutine.h"
#include <unordered_map>

using study::PHPCoroutine;
using study::Coroutine;
//无序字典user_yield_coros
static std::unordered_map<long, Coroutine*> user_yield_coros;
//  主要提供对外的接口
//声明宏定义create()
ZEND_BEGIN_ARG_INFO_EX(arginfo_study_coroutine_create, 0, 0, 1)
    ZEND_ARG_CALLABLE_INFO(0, func, 0)
ZEND_END_ARG_INFO()

//协成切换参数 yeild()
ZEND_BEGIN_ARG_INFO_EX(arginfo_study_coroutine_void, 0, 0, 0)
ZEND_END_ARG_INFO()

//协成恢复接口 resume
ZEND_BEGIN_ARG_INFO_EX(arginfo_study_coroutine_resume,0,0,1)
    ZEND_ARG_INFO(0,cid)
ZEND_END_ARG_INFO()

//判断当前协成是否存在 isExist
ZEND_BEGIN_ARG_INFO_EX(arginfo_study_coroutine_isExist, 0, 0, 1)
    ZEND_ARG_INFO(0, cid)
ZEND_END_ARG_INFO()

//协成defer
ZEND_BEGIN_ARG_INFO_EX(arginfo_study_coroutine_defer, 0, 0, 1)
    ZEND_ARG_CALLABLE_INFO(0, func, 0)
ZEND_END_ARG_INFO()
//sleep
ZEND_BEGIN_ARG_INFO_EX(arginfo_study_coroutine_sleep, 0, 0, 1)
    ZEND_ARG_INFO(0, seconds)
ZEND_END_ARG_INFO()
// 声明函数方法

PHP_FUNCTION(study_coroutine_create)
{  
     // zend_fcall_info 就是用来接收我们创建协程的时候传递的那个函数
    zend_fcall_info fci = empty_fcall_info;
    zend_fcall_info_cache fcc = empty_fcall_info_cache;  // 函数相关的信息
    zval result;

    ZEND_PARSE_PARAMETERS_START(1, -1)
        Z_PARAM_FUNC(fci, fcc)
        Z_PARAM_VARIADIC('*', fci.params, fci.param_count)
    ZEND_PARSE_PARAMETERS_END_EX(RETURN_FALSE);
    
    fci.retval = &result;
    //创建协成
    long cid =PHPCoroutine::create(&fcc, fci.param_count, fci.params);
    RETURN_LONG(cid);
}
//协成切换 
//void zim_study_coroutine_util_yield(zend_execute_data *execute_data, zval *return_value);
PHP_METHOD(study_coroutine_util, yield)
{
    Coroutine* co = Coroutine::get_current();
    user_yield_coros[co->get_cid()] = co;
    co->yield();
    RETURN_TRUE;
}
//实现协程恢复接口
PHP_METHOD(study_coroutine_util,resume) 
{
    zend_long cid; //解析参数
    ZEND_PARSE_PARAMETERS_START(1,1)
      Z_PARAM_LONG(cid);
    ZEND_PARSE_PARAMETERS_END_EX(RETURN_FALSE);
    auto coroutine_iterator = user_yield_coros.find(cid);
    if (coroutine_iterator == user_yield_coros.end())
    {
       php_error_docref(NULL, E_WARNING, "resume error");
       RETURN_FALSE;
    }
    Coroutine* co=coroutine_iterator->second;
    user_yield_coros.erase(cid); //删除字典中的协程对象
    co->resume();
    RETURN_TRUE; 
}
//获取当前的协程id
PHP_METHOD(study_coroutine_util,getCid)
{
    Coroutine * co = Coroutine::get_current();
    if(co == nullptr) {
        
          RETURN_LONG(-1);
    }
    RETURN_LONG(co->get_cid());
}
// 判断当前协成是否存在

PHP_METHOD(study_coroutine_util,isExist)  
{
    zend_long cid = 0;
    bool is_exist;
    //解析参数
    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_LONG(cid)
    ZEND_PARSE_PARAMETERS_END_EX(RETURN_FALSE);
    //去容器中查找对应的数据是否存在
    auto coroutine_iterator = Coroutine::coroutines.find(cid);
    //在容器中
    is_exist = (coroutine_iterator != Coroutine::coroutines.end());
    RETURN_BOOL(is_exist);
}
//协成defer
PHP_METHOD(study_coroutine_util,defer)
{
    zend_fcall_info fci = empty_fcall_info;
    zend_fcall_info_cache fcc = empty_fcall_info_cache;
    php_study_fci_fcc *defer_fci_fcc;
    //申请内存
    defer_fci_fcc = (php_study_fci_fcc *)emalloc(sizeof(php_study_fci_fcc));

    ZEND_PARSE_PARAMETERS_START(1, -1)
        Z_PARAM_FUNC(fci, fcc)
    ZEND_PARSE_PARAMETERS_END_EX(RETURN_FALSE);

    defer_fci_fcc->fci = fci;
    defer_fci_fcc->fcc = fcc;

    PHPCoroutine::defer(defer_fci_fcc);
}
//模拟协成阻塞IO
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
//注册当类的方法 和类绑定在一起 并且设置相应的操作权限
const zend_function_entry study_coroutine_util_methods [] = 
{   
    ZEND_FENTRY(create, ZEND_FN(study_coroutine_create), arginfo_study_coroutine_create, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
   
    PHP_ME(study_coroutine_util,yield, arginfo_study_coroutine_void, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(study_coroutine_util,resume, arginfo_study_coroutine_resume, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(study_coroutine_util, getCid, arginfo_study_coroutine_void, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(study_coroutine_util, isExist, arginfo_study_coroutine_isExist, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(study_coroutine_util, defer, arginfo_study_coroutine_defer, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)//defer
     PHP_ME(study_coroutine_util, sleep, arginfo_study_coroutine_sleep, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_FE_END // 结束的标志位
};


/**
 * Define zend class entry
 */
zend_class_entry study_coroutine_ce; //声明一个类
zend_class_entry *study_coroutine_ce_ptr; // 这个类的句柄
//协成法法初始化
 void study_coroutine_util_init()
 {  
    //初始化对应的回调函数
    PHPCoroutine::init(); // 新增的一行
      //初始化类 带命名空间的 和对应的方法   绑定类名和方法
     INIT_NS_CLASS_ENTRY(study_coroutine_ce, "Study", "Coroutine", study_coroutine_util_methods);
     // 注册对应的类 把当前类注册到CG(class_table)中 并且注册对应的方法
     study_coroutine_ce_ptr = zend_register_internal_class(&study_coroutine_ce TSRMLS_CC); // Registered in the Zend Engine
     zend_register_class_alias("SCo", study_coroutine_ce_ptr); // 新增的代码
 }