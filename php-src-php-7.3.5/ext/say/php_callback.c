#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "php_callback.h"

#include <pthread.h>  //引入线程库
//回调函数
PHP_FUNCTION(hello_callback)
{
    zval *function_name;
    zval retval;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &function_name) == FAILURE) {
        return;
    }
    if (Z_TYPE_P(function_name) != IS_STRING) {
        php_printf("Function require string argumnets!");
        return;
    }
    //TSRMLS_FETCH();
    if (call_user_function_ex(EG(function_table), NULL, function_name, &retval, 0, NULL, 0, NULL TSRMLS_CC) != SUCCESS) {
        php_printf("Function call failed!");
        return;
    }

    *return_value = retval;
    zval_copy_ctor(return_value);
    zval_ptr_dtor(&retval);

}
//线程执行的函数
static void my_thread(struct myarg *arg) 
{
    zval *fun = arg->fun;
    zval ret = arg->ret;
    if (call_user_function_ex(EG(function_table), NULL, fun, &ret, 0, NULL, 0, NULL TSRMLS_CC) != SUCCESS) {
        return;
    }
}
/**
 * 
 * 线程回调函数 hello_thread
 * @brief Construct a new php function object
 * 
 */
PHP_FUNCTION(hello_thread)
{
    pthread_t tid;
    zval *fun1, *fun2;
    zval ret1, ret2;
    struct myarg arg;
    int ret;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zz", &fun1, &fun2) == FAILURE) {
        return;
    }
    arg.fun = fun1;
    arg.ret = ret1;
    ret = pthread_create(&tid, NULL, (void*)my_thread, (void*)&arg);
    if(ret != 0) {
        php_printf("Thread Create Error\n");
        exit(0);
    }
    if (call_user_function_ex(EG(function_table), NULL, fun2, &ret2, 0, NULL, 0, NULL TSRMLS_CC) != SUCCESS) {
        return;
    }
    pthread_join(tid, NULL);
    RETURN_NULL();

}