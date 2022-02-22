#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_tinyswoole.h"

zend_class_entry *tinyswoole_ce;
zval *server_object;

ZEND_BEGIN_ARG_INFO_EX(arginfo_tinyswoole__construct, 0, 0, 2)
    ZEND_ARG_INFO(0, ip)
    ZEND_ARG_INFO(0, port)
ZEND_END_ARG_INFO()

static inline zval* tsw_zend_read_property(zend_class_entry *class_ptr, zval *obj, const char *s, int len, int silent)
{
    zval rv;
    return zend_read_property(class_ptr, obj, s, len, silent, &rv);
}
//构造方法
PHP_METHOD(tinyswoole_server, __construct)
{
    char *serv_host;
	size_t host_len;
	long serv_port;
	
	int sock;
	sayServer *serv;

	serv = say_tinyswoole_init();
    if(serv == NULL){  
           printf("say_tinyswoole_init 服务器内存申请失败\n");
           exit(1);
    }
    SaywooleG.serv = serv;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sl", &serv_host, &host_len, &serv_port) == FAILURE) {
		RETURN_NULL();
	}

	sock = saySocketCreate();
	if (sock < 0) {
		tinyswoole_php_fatal_error(E_ERROR, "tswSocket_create error");
		RETURN_NULL();
	}
	if (saySocketBind(sock,serv_host, serv_port) < 0) {
		tinyswoole_php_fatal_error(E_ERROR, "tswSocket_bind error");
		RETURN_NULL();
	}
	//serv->serv_sock = sock;

	server_object = getThis(); // server_object is a global variable
	zend_update_property_string(tinyswoole_ce, server_object, ZEND_STRL("ip"), serv_host);
	zend_update_property_long(tinyswoole_ce, server_object, ZEND_STRL("port"), serv_port);
}
//设置参数
PHP_METHOD(tinyswoole_server,set)
{
    zval *zset = NULL;
	sayServer *serv;
	HashTable *vht;
	zval *v;

	serv = SaywooleG.serv;

	ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_ARRAY(zset)
    ZEND_PARSE_PARAMETERS_END_EX(RETURN_FALSE);

	// php_tinyswoole_array_separate(zset);
    vht = Z_ARRVAL_P(zset);

	if (php_tinyswoole_array_get_value(vht, "reactor_num", v)) {
        convert_to_long(v);
        serv->reactor_num = (uint16_t)Z_LVAL_P(v);
    } else {
		serv->reactor_num = 4;
	}

	if (php_tinyswoole_array_get_value(vht, "worker_num", v)) {
        convert_to_long(v);
        serv->worker_num = (uint16_t)Z_LVAL_P(v);
    } else {
		serv->worker_num = 4;
	}
     printf("server reactor_num is %d\n", SaywooleG.serv->reactor_num);
}

PHP_METHOD(tinyswoole_server, start)
{
    zval *ip;
    zval *port;
    sayServer *serv;
    printf("tcp server running server...\n");

    printf("server worker_num is %d\n", SaywooleG.serv->worker_num);
    
    ip = tsw_zend_read_property(tinyswoole_ce, getThis(), "ip", sizeof("ip") - 1, 0);
    port = tsw_zend_read_property(tinyswoole_ce, getThis(), "port", sizeof("port") - 1, 0);

    //server(Z_STRVAL(*ip), Z_LVAL(*port));
    printf("ip is %s\n", Z_STRVAL(*ip));
}

zend_function_entry tinyswoole_method[]=
{
    ZEND_ME(tinyswoole_server, __construct, arginfo_tinyswoole__construct, ZEND_ACC_PUBLIC)
    ZEND_ME(tinyswoole_server, set, NULL, ZEND_ACC_PUBLIC)
    ZEND_ME(tinyswoole_server, start, NULL, ZEND_ACC_PUBLIC)
    {NULL, NULL, NULL}
};

void init_class_tinyswoole_untils()
{
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "tinyswoole_server", tinyswoole_method);
    tinyswoole_ce = zend_register_internal_class(&ce TSRMLS_CC);

    zend_declare_property_null(tinyswoole_ce, "ip", sizeof("ip") - 1, ZEND_ACC_PRIVATE);
    zend_declare_property_null(tinyswoole_ce, "port", sizeof("port") - 1, ZEND_ACC_PRIVATE);

}
//初始化服务类全局的对象属性
sayServer *say_tinyswoole_init()
{   
    sayServer *serv;
    serv = (sayServer *)malloc(sizeof(sayServer));
    return serv;
}
