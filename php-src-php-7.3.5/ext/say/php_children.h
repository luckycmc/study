     
#ifndef PHP_CHILDREN_H

#define PHP_CHILDREN_H

//这里要声明一个模块初始化方法
PHP_MINIT_FUNCTION(say);
PHP_MSHUTDOWN_FUNCTION(say);

//声明类的方法
PHP_METHOD(children,__construct);
PHP_METHOD(children,__destruct);
PHP_METHOD(children, learn);
PHP_METHOD(children, toString);

#endif