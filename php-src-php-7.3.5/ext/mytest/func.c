#include "func.h"
//say  函数
PHP_FUNCTION(say)
{
        zend_string *strg;
        strg = strpprintf(0,"hello world");
        RETURN_STR(strg);
}