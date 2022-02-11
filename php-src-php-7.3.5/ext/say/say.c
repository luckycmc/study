/* say extension for PHP */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "php_say.h"
/*******say简单的函数*****/
PHP_FUNCTION(say)
{
    zend_string *strg;
    strg = strpprintf(0, "hello word 扩展 method\n");
    RETURN_STR(strg);
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
	PHP_FE_END
};
/* }}} */

/* {{{ say_module_entry
 */
zend_module_entry say_module_entry = {
	STANDARD_MODULE_HEADER,
	"say",					/* Extension name */
	say_functions,			/* zend_function_entry */
	NULL,							/* PHP_MINIT - Module initialization */
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

