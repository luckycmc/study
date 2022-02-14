/* say extension for PHP */

#ifndef PHP_SAY_H
# define PHP_SAY_H

extern zend_module_entry say_module_entry;
# define phpext_say_ptr &say_module_entry

# define PHP_SAY_VERSION "0.1.0"

# if defined(ZTS) && defined(COMPILE_DL_SAY)
ZEND_TSRMLS_CACHE_EXTERN()
# endif


//这里要声明一个模块初始化方法
PHP_MINIT_FUNCTION(minho);
PHP_MSHUTDOWN_FUNCTION(minho);

//声明类的方法
PHP_METHOD(children,__construct);
PHP_METHOD(children,__destruct);
PHP_METHOD(children, learn);
PHP_METHOD(children, toString);

#endif	/* PHP_SAY_H */

