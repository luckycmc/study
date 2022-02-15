     
#ifndef PHP_CHILDREN_H

#define PHP_CHILDREN_H

//声明类的方法
PHP_METHOD(children,__construct);
PHP_METHOD(children,__destruct);
PHP_METHOD(children, learn);
PHP_METHOD(children, toString);

void init_class_untils();

#endif