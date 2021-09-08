/* hello_world extension for PHP */

#ifndef PHP_HELLO_WORLD_H
# define PHP_HELLO_WORLD_H

extern zend_module_entry hello_world_module_entry;
# define phpext_hello_world_ptr &hello_world_module_entry

# define PHP_HELLO_WORLD_VERSION "0.1.0"

# if defined(ZTS) && defined(COMPILE_DL_HELLO_WORLD)
ZEND_TSRMLS_CACHE_EXTERN()
# endif

#endif	/* PHP_HELLO_WORLD_H */

