/* tinyswoole extension for PHP */

#ifndef PHP_TINYSWOOLE_H
# define PHP_TINYSWOOLE_H

extern zend_module_entry tinyswoole_module_entry;
# define phpext_tinyswoole_ptr &tinyswoole_module_entry

# define PHP_TINYSWOOLE_VERSION "0.1.0"

# if defined(ZTS) && defined(COMPILE_DL_TINYSWOOLE)
ZEND_TSRMLS_CACHE_EXTERN()
# endif

#endif	/* PHP_TINYSWOOLE_H */

