#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"

#include "php_types.h"  //必须引入对应的文件头 否者编译会出现错误

PHP_FUNCTION(get_size)
{
    zval *val;
    zend_string *result;
    HashTable *myht;

#ifndef FAST_ZPP
    if(zend_parse_parameters(ZEND_NUM_ARGS(),"z",&val) == FAILURE){
        return;
    }
#else
    ZEND_PARSE_PARAMETERS_START(1,1)
        Z_PARAM_ZVAL_EX(val,0,1)
    ZEND_PARSE_PARAMETERS_END();
#endif

    if(Z_TYPE_P(val) == IS_STRING){
         result = strpprintf(0, "string size is %d", Z_STRLEN_P(val));
    }else if (Z_TYPE_P(val) == IS_ARRAY) {
        myht = Z_ARRVAL_P(val);
        result = strpprintf(0, "array size is %d", zend_array_count(myht));
    } else {
        result = strpprintf(0, "can not support");
    }   
    RETURN_STR(result);
}


//gettype()来取得变量的类型的hello_typeof(): 判断对应变量的类型
PHP_FUNCTION(hello_typeof)
{
     zval *userval = NULL;
     if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z",&userval) == FAILURE){
         RETURN_NULL();
     }

     switch(Z_TYPE_P(userval))
     {
        case IS_NULL:
              RETVAL_STRING("NULL");
              break;
        case IS_TRUE:
            RETVAL_STRING("true");
            break;

        case IS_FALSE:
            RETVAL_STRING("false");
            break;

        case IS_LONG:
            RETVAL_STRING("integer");
            break;

        case IS_DOUBLE:
            RETVAL_STRING("double");
            break;

        case IS_STRING:
            RETVAL_STRING("string");
            break;

        case IS_ARRAY:
            RETVAL_STRING("array");
            break;

        case IS_OBJECT:
            RETVAL_STRING("object");
            break;

        case IS_RESOURCE:
            RETVAL_STRING("resource");
            break;

        default:
            RETVAL_STRING("unknown type");
     }
}

/*****
 php 数组的实现
 * **/
PHP_FUNCTION(hello_get_arr)
{
    array_init(return_value);
    add_next_index_null(return_value);
    add_next_index_long(return_value, 42);
    add_next_index_bool(return_value, 1);
    add_next_index_double(return_value, 3.14);
    add_next_index_string(return_value, "foo");
    add_assoc_string(return_value, "mno", "baz");
    add_assoc_bool(return_value, "ghi", 1);
}