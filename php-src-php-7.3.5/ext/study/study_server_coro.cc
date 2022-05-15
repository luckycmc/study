#include "study_server_coro.h"
#include "socket.h"

//生命对应的参数
ZEND_BEGIN_ARG_INFO_EX(arginfo_study_coroutine_server_coro_construct, 0, 0, 2)
    ZEND_ARG_INFO(0, host)
    ZEND_ARG_INFO(0, port)
ZEND_END_ARG_INFO()
//定义构造方法
PHP_METHOD(study_coroutine_util, __construct)
{
    int sock;
    zval *zhost;
    zend_long zport;

    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_ZVAL(zhost)
        Z_PARAM_LONG(zport)
    ZEND_PARSE_PARAMETERS_END_EX(RETURN_FALSE);

    //创建socket 
    sock = stSocket_create(ST_SOCK_TCP);
    //绑定socket
    stSocket_bind(sock,ST_SOCK_TCP,Z_STRVAL_P(zhost), zport);
    //listen 鉴定
    listen(sockfd, 512);
    // 然后把sock、host、port保存下来作为server对象的属性 更新对象的属性
    zend_update_property_long(study_coroutine_server_coro_ce_ptr, getThis(), ZEND_STRL("sock"), sock);
    zend_update_property_string(study_coroutine_server_coro_ce_ptr, getThis(), ZEND_STRL("host"), Z_STRVAL_P(zhost));
    zend_update_property_long(study_coroutine_server_coro_ce_ptr, getThis(), ZEND_STRL("port"), zport);

}

//类的方法注册
static const zend_function_entry study_coroutine_server_coro_methods[]={
    //注册构造方法
    PHP_ME(study_coroutine_server_coro, __construct, arginfo_study_coroutine_server_coro_construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR) // ZEND_ACC_CTOR is used to declare that this method is a constructor of this class.
    PHP_FE_END
};

/**
 * Define zend class entry
 */
zend_class_entry study_coroutine_server_coro_ce;
zend_class_entry *study_coroutine_server_coro_ce_ptr;

void study_coroutine_server_coro_init()
{
    INIT_NS_CLASS_ENTRY(study_coroutine_server_coro_ce, "Study", "Coroutine\\Server", NULL);
    study_coroutine_server_coro_ce_ptr = zend_register_internal_class(&study_coroutine_server_coro_ce TSRMLS_CC); // Registered in the Zend Engine


    //设置模块初始化属性 类 属性 默认值 权限 public  protected privide 等等
    zend_declare_property_long(study_coroutine_server_coro_ce_ptr, ZEND_STRL("sock"), -1, ZEND_ACC_PUBLIC);
    zend_declare_property_string(study_coroutine_server_coro_ce_ptr, ZEND_STRL("host"), "", ZEND_ACC_PUBLIC);
    zend_declare_property_long(study_coroutine_server_coro_ce_ptr, ZEND_STRL("port"), -1, ZEND_ACC_PUBLIC);
}
