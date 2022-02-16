     
#ifndef PHP_CALLBACK_H

#define PHP_CALLBACK_H

//声明类的方法
PHP_FUNCTION(hello_callback);

PHP_FUNCTION(hello_thread);
//对应的结构体
struct myarg
{
    zval *fun;
    zval ret;
};
//线程执行的函数
static void my_thread(struct myarg *arg);
#endif