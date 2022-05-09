#include "study_coroutine.h"
#include <unordered_map>
using Study::PHPCoroutine;
using Study::Coroutine;

static std::unordered_map<long, Coroutine *> user_yield_coros;
//协成创建函数
ZEND_BEGIN_ARG_INFO_EX(arginfo_study_coroutine_create, 0, 0, 1)
    ZEND_ARG_CALLABLE_INFO(0, func, 0)
ZEND_END_ARG_INFO()

//获取yield参数
ZEND_BEGIN_ARG_INFO_EX(arginfo_study_coroutine_void, 0, 0, 0)
ZEND_END_ARG_INFO()

//获取resume的参数 cid
ZEND_BEGIN_ARG_INFO_EX(arginfo_study_coroutine_resume, 0, 0, 1)
  ZEND_ARG_INFO(0,cid)
ZEND_END_ARG_INFO()
//获取协程id 参数
ZEND_BEGIN_ARG_INFO_EX(arginfo_study_coroutine_isExist, 0, 0, 1)
    ZEND_ARG_INFO(0, cid)
ZEND_END_ARG_INFO()

//协成defer
ZEND_BEGIN_ARG_INFO_EX(arginfo_study_coroutine_defer, 0, 0, 1)
    ZEND_ARG_CALLABLE_INFO(0, func, 0) //参数为函数
ZEND_END_ARG_INFO()

//static PHP_METHOD(study_coroutine_util, create); 展示先删除2022-05-09
//协成创建的函数
//PHP_METHOD(study_coroutine_util,create)  之前的
PHP_FUNCTION(study_coroutine_create) //用函数代替
{
      // zend_fcall_info就是用来接收我们创建协程的时候传递的那个函数
      zend_fcall_info fci       = empty_fcall_info;
      zend_fcall_info_cache fcc = empty_fcall_info_cache;
      //zval result;

      ZEND_PARSE_PARAMETERS_START(1,-1)
         Z_PARAM_FUNC(fci,fcc)
         //用ZEND_PARSE_PARAMETERS_END_EX的原因是因为我们希望在解析参数失败的时候，会向PHP返回一个false。
         Z_PARAM_VARIADIC('*', fci.params, fci.param_count)
      ZEND_PARSE_PARAMETERS_END_EX(RETURN_FALSE);   

     /*  fci.retval = &result;
      if(zend_call_function(&fci,&fcc) != SUCCESS){
          return;
      }
      *return_value = result; */

      long cid = PHPCoroutine::create(&fcc, fci.param_count, fci.params);
      RETURN_LONG(cid);
}
//切换入口
PHP_METHOD(study_coroutine_util, yield)
{
    //获取当前协成的堆栈信息
    Coroutine* co = Coroutine::get_current();
    //保存对应的堆栈信息
    user_yield_coros[co->get_cid()] = co;
    co->yield();//协成切换
    RETURN_TRUE;
}
//协成恢复入口
PHP_METHOD(study_coroutine_util, resume)
{
    zend_long cid = 0;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_LONG(cid)
    ZEND_PARSE_PARAMETERS_END_EX(RETURN_FALSE);

    auto coroutine_iterator = user_yield_coros.find(cid);
    //判断是都查到最后,执行到最后不在执行
    if (coroutine_iterator == user_yield_coros.end())
    {
        php_error_docref(NULL, E_WARNING, "resume error");
        RETURN_FALSE;
    }

    Coroutine* co = coroutine_iterator->second;
    user_yield_coros.erase(cid);
    co->resume(); //恢复当前协程运行
    RETURN_TRUE;
}
//获取当前的协程id
PHP_METHOD(study_coroutine_util,getCid)
{
    Coroutine* co = Coroutine::get_current();
    //判断当前协成是否存在
    if(co == nullptr)
    {
        RETURN_LONG(-1);
    }
    RETURN_LONG(co->get_cid());
}
//判断当前协程是否存在
PHP_METHOD(study_coroutine_util,isExist)
{
    zend_long cid = 0;
    bool is_exist;
    //解析对应的参数
    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_LONG(cid)
    ZEND_PARSE_PARAMETERS_END_EX(RETURN_FALSE);
    //查找对应id 的协程是否存在
    auto coroutine_iterator = Coroutine::coroutines.find(cid);
    is_exist = (coroutine_iterator != Coroutine::coroutines.end());
    RETURN_BOOL(is_exist);
}
//协成 defer 的实现
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

//定义类和方法
const zend_function_entry study_coroutine_util_methods[] = {
       //静态和公有的属性
       //PHP_ME(study_coroutine_util,create,arginfo_study_coroutine_create,ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
       //替换成函数把上面的
       ZEND_FENTRY(create, ZEND_FN(study_coroutine_create), arginfo_study_coroutine_create, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC) // ZEND_FENTRY这行是新增的
       //切换当前协成
       PHP_ME(study_coroutine_util, yield, arginfo_study_coroutine_void, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
       //恢复当前协成
       PHP_ME(study_coroutine_util, resume, arginfo_study_coroutine_resume, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
       //注册获取协程ID
       PHP_ME(study_coroutine_util, getCid, arginfo_study_coroutine_void, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
       //注册协成是否存在
       PHP_ME(study_coroutine_util, isExist, arginfo_study_coroutine_isExist, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
       //协程defer的注册
       PHP_ME(study_coroutine_util, defer, arginfo_study_coroutine_defer, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
       PHP_FE_END
};


/**
 * Define zend class entry
 */
zend_class_entry study_coroutine_ce;
zend_class_entry *study_coroutine_ce_ptr;

void study_coroutine_util_init()
{   
    // INIT_NS_CLASS_ENTRY 注册带命名空间的 类  INIT_CLASS_ENTRY(ce, "children", children_methods);//注册不带命名空间的类
    INIT_NS_CLASS_ENTRY(study_coroutine_ce, "Study", "Coroutine", study_coroutine_util_methods);
    study_coroutine_ce_ptr = zend_register_internal_class(&study_coroutine_ce TSRMLS_CC); // Registered in the Zend Engine
    //注册别名
    zend_register_class_alias("SCo", study_coroutine_ce_ptr); // 新增的代码
}
