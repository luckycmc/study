#ifndef PHP_STUDY_H
#define PHP_STUDY_H

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"

#include "study.h"

#define PHP_STUDY_VERSION "0.1.0"

extern zend_module_entry study_module_entry;
#define phpext_study_ptr &study_module_entry

#ifdef PHP_WIN32
#    define PHP_STUDY_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#    define PHP_STUDY_API __attribute__ ((visibility("default")))
#else
#    define PHP_STUDY_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif
//协成类初始化
void study_coroutine_util_init();      //协成公用的方法
void study_coroutine_server_coro_init(); //服务器初始化
void study_coro_channel_init(); //channel 初始化

// st_zend_read_property是用来读取对象的某个属性的函数
inline zval *st_zend_read_property(zend_class_entry *class_ptr, zval *obj, const char *s, int len, int silent)
{
    zval rv;
    return zend_read_property(class_ptr, obj, s, len, silent, &rv);
}
//开辟堆栈
inline zval* st_malloc_zval()
{
    return (zval *) emalloc(sizeof(zval));
}
//cpoy 数据
inline zval* st_zval_dup(zval *val)
{
    zval *dup = st_malloc_zval();
    memcpy(dup, val, sizeof(zval));
    return dup;
}
/**宏定义 start**/
#define ST_SET_CLASS_CREATE(module, _create_object) \
    module##_ce_ptr->create_object = _create_object

#define ST_SET_CLASS_FREE(module, _free_obj) \
    module##_handlers.free_obj = _free_obj

#define ST_SET_CLASS_CREATE_AND_FREE(module, _create_object, _free_obj) \
    ST_SET_CLASS_CREATE(module, _create_object); \
    ST_SET_CLASS_FREE(module, _free_obj)

/**
 * module##_handlers.offset 保存PHP对象在自定义对象中的偏移量
 */
#define ST_SET_CLASS_CUSTOM_OBJECT(module, _create_object, _free_obj, _struct, _std) \
    ST_SET_CLASS_CREATE_AND_FREE(module, _create_object, _free_obj); \
    module##_handlers.offset = XtOffsetOf(_struct, _std)
/**宏定义 end**/
/**
 * Declare any global variables you may need between the BEGIN and END macros here
 */
ZEND_BEGIN_MODULE_GLOBALS(study)

ZEND_END_MODULE_GLOBALS(study)

#endif    /* PHP_STUDY_H */


