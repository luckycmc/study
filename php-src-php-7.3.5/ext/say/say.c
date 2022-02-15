/* say extension for PHP */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "php_say.h"

#include "php_types.h"  //必须引入对应的文件头 否者编译会出现错误

#include "php_children.h"
/*******say简单的函数*****/
PHP_FUNCTION(say)
{
    zend_string *strg;
    strg = strpprintf(0, "hello word 扩展 method\n");
    RETURN_STR(strg);  //函数返回字符串
}
/**for_say扩展**/
PHP_FUNCTION(for_say)
{    
	int i =0;
	while(1)
	{  
		if(i >= 10){
            break;
		}
        printf("for_say\n");
		i++;

	}
    
}
/**带参数的函数**/
PHP_FUNCTION(my_function) 
{
    char *arg = NULL;
    size_t arg_len;
    // 解析对应的参数
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s",&arg, &arg_len) == FAILURE){
           return;
    }
    zend_string *strg;
    strg = strpprintf(0,arg);
    RETURN_STR(strg);
}

/*****数据的加法***/

PHP_FUNCTION(smart_counter)
{
    long arg_add1 = 0;
    long arg_add2 = 0;
    long sum = 0;
    size_t arg_len;
    /**解析出对应的参数 两个长整形的参数**/
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "ll", &arg_add1, &arg_add2, &arg_len) == FAILURE) {
        return;
    }
  
    sum = arg_add1 + arg_add2;
  
    RETURN_LONG(sum);
}
/***函数类型 start**/

PHP_FUNCTION(default_value)
{
    zend_string     *type;    
    zval            *value = NULL;
 
#ifndef FAST_ZPP
    /* Get function parameters and do error-checking. */
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "S|z", &type, &value) == FAILURE) {
        return;
    }    
#else
    //第一个参数表示必传的参数个数，第二个参数表示最多传入的参数个数。
    ZEND_PARSE_PARAMETERS_START(1, 2)
        Z_PARAM_STR(type)   //Z_PARAM_STR	对应 S
        Z_PARAM_OPTIONAL     // Z_PARAM_OPTIONAL 对应|
        Z_PARAM_ZVAL_EX(value, 0, 1)
    ZEND_PARSE_PARAMETERS_END(); //ZEND_PARSE_PARAMETERS_END();结束。
#endif
     
    if (ZSTR_LEN(type) == 3 && strncmp(ZSTR_VAL(type), "int", 3) == 0 && value == NULL) {
        RETURN_LONG(0);
    } else if (ZSTR_LEN(type) == 3 && strncmp(ZSTR_VAL(type), "int", 3) == 0 && value != NULL) {
        RETURN_ZVAL(value, 0, 1); 
    } else if (ZSTR_LEN(type) == 4 && strncmp(ZSTR_VAL(type), "bool", 4) == 0 && value == NULL) {
        RETURN_FALSE;
    } else if (ZSTR_LEN(type) == 4 && strncmp(ZSTR_VAL(type), "bool", 4) == 0 && value != NULL) {
        RETURN_ZVAL(value, 0, 1); 
    } else if (ZSTR_LEN(type) == 3 && strncmp(ZSTR_VAL(type), "str", 3) == 0 && value == NULL) {
        RETURN_EMPTY_STRING();
    } else if (ZSTR_LEN(type) == 3 && strncmp(ZSTR_VAL(type), "str", 3) == 0 && value != NULL) {
        RETURN_ZVAL(value, 0, 1); 
    } 
    RETURN_NULL();
}
/**类型处理**/
PHP_FUNCTION(getSize)
{
    zval *val;
	size_t size;
	zend_string *result;
	HashTable *myht;

	/*******开始解析参数*******/
	if(zend_parse_parameters(ZEND_NUM_ARGS(),"z",&val)== FAILURE){
		return;
	}
	//判断类型参数
	if(Z_TYPE_P(val) == IS_STRING){
          result = strpprintf(0, "string size is %d", Z_STRLEN_P(val));
	}else if (Z_TYPE_P(val) == IS_ARRAY){
          
		   myht = Z_ARRVAL_P(val);
		   result = strpprintf(0, "array size is %d", zend_array_count(myht));
	}else{
		result = strpprintf(0, "can not support current type");
	}
	RETURN_STR(result);
}



/* For compatibility with older PHP versions */
#ifndef ZEND_PARSE_PARAMETERS_NONE
#define ZEND_PARSE_PARAMETERS_NONE() \
	ZEND_PARSE_PARAMETERS_START(0, 0) \
	ZEND_PARSE_PARAMETERS_END()
#endif

/* {{{ void say_test1()
 */
PHP_FUNCTION(say_test1)
{
	ZEND_PARSE_PARAMETERS_NONE();

	php_printf("The extension %s is loaded and working!\r\n", "say");
}
/* }}} */

/* {{{ string say_test2( [ string $var ] )
 */
PHP_FUNCTION(say_test2)
{
	char *var = "World";
	size_t var_len = sizeof("World") - 1;
	zend_string *retval;

	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_STRING(var, var_len)
	ZEND_PARSE_PARAMETERS_END();

	retval = strpprintf(0, "Hello %s", var);

	RETURN_STR(retval);
}
/* }}}*/

/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(say)
{
#if defined(ZTS) && defined(COMPILE_DL_SAY)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif

	return SUCCESS;
}
/* }}} */

//当模块启动时执行的方法 
/******
 犯了一个错误 没有吧初始化加入到模块中导致类的加载不存在
 * */
PHP_MINIT_FUNCTION(say)
{   
    init_class_untils();
    return SUCCESS;
}
PHP_MSHUTDOWN_FUNCTION(say)
{
    return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(say)
{
    return SUCCESS;
}

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(say)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "say support", "enabled");
	php_info_print_table_end();
}
/* }}} */

/* {{{ arginfo
 */
ZEND_BEGIN_ARG_INFO(arginfo_say_test1, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_say_test2, 0)
	ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ say_functions[]
 */
static const zend_function_entry say_functions[] = {
	PHP_FE(say, NULL)    /* For testing, remove later. */
	PHP_FE(say_test1,		arginfo_say_test1)
	PHP_FE(say_test2,		arginfo_say_test2)
	PHP_FE(for_say,NULL) /* For testing, remove later. */
	PHP_FE(my_function,NULL) /* For testing, remove later. */
	PHP_FE(smart_counter,NULL) /* For testing, remove later. */
	PHP_FE(getSize,NULL) /* For testing, remove later. */
    PHP_FE(get_size,NULL) /* For testing, remove later. */
	PHP_FE_END
};
/* }}} */

/* {{{ say_module_entry
 */
zend_module_entry say_module_entry = {
	STANDARD_MODULE_HEADER,
	"say",					/* Extension name */
	say_functions,			/* zend_function_entry */
	PHP_MINIT(say),						/* PHP_MINIT - Module initialization */
	NULL,							/* PHP_MSHUTDOWN - Module shutdown */
	PHP_RINIT(say),			/* PHP_RINIT - Request initialization */
	NULL,							/* PHP_RSHUTDOWN - Request shutdown */
	PHP_MINFO(say),			/* PHP_MINFO - Module info */
	PHP_SAY_VERSION,		/* Version */
	STANDARD_MODULE_PROPERTIES
};
/* }}} */



#ifdef COMPILE_DL_SAY
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(say)
#endif

