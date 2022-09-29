/* mytest extension for PHP */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "php_mytest.h"

/* For compatibility with older PHP versions */
#ifndef ZEND_PARSE_PARAMETERS_NONE
#define ZEND_PARSE_PARAMETERS_NONE() \
	ZEND_PARSE_PARAMETERS_START(0, 0) \
	ZEND_PARSE_PARAMETERS_END()
#endif
#include "func.h"
/*****************************测试函数 start****************/
//解析参数
PHP_FUNCTION(zpw_long)
{
	zend_long i;
	//解析参数 并赋值给i
	if(zend_parse_parameters(ZEND_NUM_ARGS(),"l",&i) == FAILURE){
           
		     RETURN_FALSE;
	}
	RETURN_LONG(i);

}
//zai函数中调用用户空间的函数
PHP_FUNCTION(zpw_handle)
{
    zend_long i;
	zval call_func_name,call_user_ret,call_func_params[1];  //函数名,返回值,参数
	uint32_t call_func_param_cnt = 1;
	zend_string *call_func_str;
	char *func_name = "mySum";  //调用用户空间的函数
	// 解析参数
	if (zend_parse_parameters(ZEND_NUM_ARGS(),"l",&i) == FAILURE)
	{
		RETURN_FALSE;
	}
	//分配zend_string
	call_func_str = zend_string_init(func_name,strlen(func_name),0);
	//设置zval
	ZVAL_STR(&call_func_name, call_func_str);
	//设置参数
	ZVAL_LONG(&call_func_params[0],i);
	//调用函数 call
	if (SUCCESS != call_user_function(EG(function_table),NULL,&call_func_name, &call_user_ret, call_func_param_cnt, call_func_params))
	{
		 zend_string_release(call_func_str); //释放字符串
         RETURN_FALSE;
	}
	zend_string_release(call_func_str); //释放字符串
    RETURN_LONG(Z_LVAL(call_user_ret));
}
/*****************************测试函数 end****************/
/* {{{ void mytest_test1()
 */
PHP_FUNCTION(mytest_test1)
{
	ZEND_PARSE_PARAMETERS_NONE();

	php_printf("The extension %s is loaded and working!\r\n", "mytest");
}
/* }}} */

/* {{{ string mytest_test2( [ string $var ] )
 */
PHP_FUNCTION(mytest_test2)
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
//加载模块启动函数
PHP_MINIT_FUNCTION(study)
{
    return SUCCESS;
}

/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(mytest)
{
#if defined(ZTS) && defined(COMPILE_DL_MYTEST)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(mytest)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "mytest support", "enabled");
	php_info_print_table_end();
}
/* }}} */

/* {{{ arginfo
 */
ZEND_BEGIN_ARG_INFO(arginfo_mytest_test1, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_mytest_test2, 0)
	ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ mytest_functions[]  注册函数
 */
static const zend_function_entry mytest_functions[] = {
	PHP_FE(mytest_test1,		arginfo_mytest_test1)
	PHP_FE(mytest_test2,		arginfo_mytest_test2)
	PHP_FE(say,		NULL)
	PHP_FE(zpw_long,NULL)
	PHP_FE(zpw_handle,NULL)
	PHP_FE_END
};
// 以 say为例 注册的是zif_say name 对应的是handler也就是say函数
//{ #zend_name, name, arg_info, (uint32_t) (sizeof(arg_info)/sizeof(struct _zend_internal_arg_info)-1), flags }
/* }}} */

/* {{{ mytest_module_entry
 */
zend_module_entry mytest_module_entry = {
	STANDARD_MODULE_HEADER,
	"mytest",					/* Extension name */
	mytest_functions,			/* zend_function_entry */
	NULL,							/* PHP_MINIT - Module initialization */
	NULL,							/* PHP_MSHUTDOWN - Module shutdown */
	PHP_RINIT(mytest),			/* PHP_RINIT - Request initialization */
	NULL,							/* PHP_RSHUTDOWN - Request shutdown */
	PHP_MINFO(mytest),			/* PHP_MINFO - Module info */
	PHP_MYTEST_VERSION,		/* Version */
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_MYTEST
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(mytest)
#endif

