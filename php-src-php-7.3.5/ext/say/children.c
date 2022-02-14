#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "php_children.h"
/************类的创建 start*************/
//定义全局类对象
zend_class_entry *children_ce;

//定义learn方法接受的参数
ZEND_BEGIN_ARG_INFO_EX(arginfo_children_learn, 0, 0, 1)
                ZEND_ARG_INFO(0, love)
ZEND_END_ARG_INFO()
//定义无参数的信息
ZEND_BEGIN_ARG_INFO(arginfo_return__void, 0)
ZEND_END_ARG_INFO()


//定义learn方法
PHP_METHOD(children, learn)
{
    char *love;
    size_t love_len;

#ifndef FAST_ZPP
    if (zend_parse_method_parameters(ZEND_NUM_ARGS(), getThis(), "s",&love, &love_len) == FAILURE) {
        return;
    }
#else
    ZEND_PARSE_PARAMETERS_START(1,1)
            Z_PARAM_STRING(love,love_len)
    ZEND_PARSE_PARAMETERS_END();
#endif
    zend_update_property_string(children_ce,  getThis(), "memory", sizeof("memory") - 1, love);

}
//定义toString方法，无参，只打印字符串
PHP_METHOD(children,toString)
{
    php_printf("can not support");
}
//定义构造方法
PHP_METHOD(children,__construct){
    php_printf("construct is running<br>");
}
//定义children对象的方法列表，可声明方法为静态方法或公开方法
const zend_function_entry children_methods[] = {
        ZEND_ME(children, learn, arginfo_children_learn, ZEND_ACC_PUBLIC )
        ZEND_ME(children,toString,arginfo_return__void,ZEND_ACC_PUBLIC | ZEND_ACC_STATIC )
        ZEND_ME(children,__construct,arginfo_return__void, ZEND_ACC_PUBLIC)
        {NULL, NULL, NULL}
};
//当模块启动时执行的方法
PHP_MINIT_FUNCTION(say)
{
    zend_class_entry ce;
    //初始化一个类对象，并将方法绑定到对象上
    INIT_CLASS_ENTRY(ce, "children", children_methods);
	//将对象赋值给全局对象
    children_ce = zend_register_internal_class_ex(&ce,NULL);
	//初始化类的属性
    zend_declare_property_null(children_ce, "memory", sizeof("memory") - 1, ZEND_ACC_PUBLIC);

    return SUCCESS;
}
/************类的创建 end**************/