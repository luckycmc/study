     
#ifndef PHP_CALLBACK_H

#define PHP_CALLBACK_H

#define TSW_OK 0
#define TSW_ERR -1

#define tinyswoole_php_fatal_error(level, fmt_str, ...)		php_error_docref(NULL TSRMLS_CC, level, fmt_str, ##__VA_ARGS__)


#define php_tinyswoole_array_get_value(ht, str, v)     ((v = zend_hash_str_find(ht, str, sizeof(str)-1)) && !ZVAL_IS_NULL(v))

//服务器结构体
typedef struct _sayServer sayServer;
typedef struct _Global_sayServer Global_sayServer;


void init_class_tinyswoole_untils();
//服务变量全局初始化
sayServer *say_tinyswoole_init();

struct _sayServer 
{
     int worker_num;
     int reactor_num;
     int reactor_pipe_num;
};
//全局服务器对象
struct _Global_sayServer{
     sayServer *serv;
};

Global_sayServer SaywooleG;

#endif