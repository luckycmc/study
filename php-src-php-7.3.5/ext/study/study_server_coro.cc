#include "study_server_coro.h"
#include "log.h"

using study::coroutine::Socket;

ZEND_BEGIN_ARG_INFO_EX(arginfo_study_coroutine_void, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_study_coroutine_server_coro_construct, 0, 0, 2)
    ZEND_ARG_INFO(0, host)
    ZEND_ARG_INFO(0, port)
ZEND_END_ARG_INFO()
// recv 参数
ZEND_BEGIN_ARG_INFO_EX(arginfo_study_coroutine_server_coro_recv, 0, 0, 2)
    ZEND_ARG_INFO(0, fd)
    ZEND_ARG_INFO(0, length)
ZEND_END_ARG_INFO()
// send 参数
ZEND_BEGIN_ARG_INFO_EX(arginfo_study_coroutine_server_coro_send, 0, 0, 2)
    ZEND_ARG_INFO(0, fd)
    ZEND_ARG_INFO(0, data)
ZEND_END_ARG_INFO()
// 关闭参数
ZEND_BEGIN_ARG_INFO_EX(arginfo_study_coroutine_server_coro_close, 0, 0, 1)
    ZEND_ARG_INFO(0, fd)
ZEND_END_ARG_INFO()

/**
 * Define zend class entry
 */
zend_class_entry study_coroutine_server_coro_ce;
zend_class_entry *study_coroutine_server_coro_ce_ptr;
//构造方法接口
PHP_METHOD(study_coroutine_server_coro, __construct)
{
    zval *zhost;
    zend_long zport;
    zval zsock;

    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_ZVAL(zhost)
        Z_PARAM_LONG(zport)
    ZEND_PARSE_PARAMETERS_END_EX(RETURN_FALSE);
    //使用协成话socket
    Socket *sock = new Socket(AF_INET, SOCK_STREAM, 0);

    sock->bind(ST_SOCK_TCP, Z_STRVAL_P(zhost), zport);
    sock->listen();
    //更新当前类的属性
    ZVAL_PTR(&zsock, sock);

    zend_update_property(study_coroutine_server_coro_ce_ptr, getThis(), ZEND_STRL("zsock"), &zsock);
    zend_update_property_string(study_coroutine_server_coro_ce_ptr, getThis(), ZEND_STRL("host"), Z_STRVAL_P(zhost));
    zend_update_property_long(study_coroutine_server_coro_ce_ptr, getThis(), ZEND_STRL("port"), zport);
}
//接受客户端连接
PHP_METHOD(study_coroutine_server_coro, accept)
{
    zval *zsock;
    Socket *sock;
    int connfd;
    // 读取到我们在构造函数里面设置的Socket对象。然后，我们再调用这个对象的accept方法
    zsock = st_zend_read_property(study_coroutine_server_coro_ce_ptr, getThis(), ZEND_STRL("zsock"), 0);
    sock = (Socket *)Z_PTR_P(zsock);
    connfd = sock->accept();
    RETURN_LONG(connfd);
}
//接受客户端发送的数据
PHP_METHOD(study_coroutine_server_coro, recv)
{
    ssize_t ret;
    zend_long fd;
    zend_long length = 65536;

    ZEND_PARSE_PARAMETERS_START(1, 2)
        Z_PARAM_LONG(fd)
        Z_PARAM_OPTIONAL
        Z_PARAM_LONG(length)
    ZEND_PARSE_PARAMETERS_END_EX(RETURN_FALSE);

    Socket::init_read_buffer();// 初始化读取buffer

    Socket *conn = new Socket(fd);
    ret = conn->recv(Socket::read_buffer, Socket::read_buffer_len);
    if (ret == 0) //客户端关闭
    {
        zend_update_property_long(study_coroutine_server_coro_ce_ptr, getThis(), ZEND_STRL("errCode"), ST_ERROR_SESSION_CLOSED_BY_CLIENT);
        zend_update_property_string(study_coroutine_server_coro_ce_ptr, getThis(), ZEND_STRL("errMsg"), st_strerror(ST_ERROR_SESSION_CLOSED_BY_CLIENT));
        RETURN_FALSE;
    }
    if (ret < 0) //出现错误
    {
        php_error_docref(NULL, E_WARNING, "recv error");
        RETURN_FALSE;
    }
    delete conn; //释放当前对象
    Socket::read_buffer[ret] = '\0';
    RETURN_STRING(Socket::read_buffer);
}
//发送数据给客户端
PHP_METHOD(study_coroutine_server_coro, send)
{
    ssize_t ret;
    zend_long fd;
    char *data;
    size_t length;

    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_LONG(fd)
        Z_PARAM_STRING(data, length)
    ZEND_PARSE_PARAMETERS_END_EX(RETURN_FALSE);

    Socket *conn = new Socket(fd);
    ret = conn->send(data, length);// 发送数据给客户端
    if (ret < 0)
    {
        php_error_docref(NULL, E_WARNING, "send error");
        RETURN_FALSE;
    }
    delete conn; //删除这个对象
    RETURN_LONG(ret);
}
//关闭服务器
PHP_METHOD(study_coroutine_server_coro, close)
{
    int ret;
    zend_long fd;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_LONG(fd)
    ZEND_PARSE_PARAMETERS_END_EX(RETURN_FALSE);

    if (fd == 4)
    {
        stError("close 4");
    }

    Socket *sock = new Socket(fd);
    ret = sock->close(); //关闭客户端
    if (ret < 0)
    {
        php_error_docref(NULL, E_WARNING, "close error");
        RETURN_FALSE;
    }
    delete sock;
    RETURN_LONG(ret);
}
//注册类的函数
static const zend_function_entry study_coroutine_server_coro_methods[] =
{
    PHP_ME(study_coroutine_server_coro, __construct, arginfo_study_coroutine_server_coro_construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR) // ZEND_ACC_CTOR is used to declare that this method is a constructor of this class.
    PHP_ME(study_coroutine_server_coro, accept, arginfo_study_coroutine_void, ZEND_ACC_PUBLIC)
    PHP_ME(study_coroutine_server_coro, recv, arginfo_study_coroutine_server_coro_recv, ZEND_ACC_PUBLIC)
    PHP_ME(study_coroutine_server_coro, send, arginfo_study_coroutine_server_coro_send, ZEND_ACC_PUBLIC)
    PHP_ME(study_coroutine_server_coro, close, arginfo_study_coroutine_server_coro_close, ZEND_ACC_PUBLIC)
    PHP_FE_END
};
// server  初始化
void study_coroutine_server_coro_init()
{
    zval zsock;
    //注册带命名空间的类
    INIT_NS_CLASS_ENTRY(study_coroutine_server_coro_ce, "Study", "Coroutine\\Server", study_coroutine_server_coro_methods);
    study_coroutine_server_coro_ce_ptr = zend_register_internal_class(&study_coroutine_server_coro_ce TSRMLS_CC); // Registered in the Zend Engine
   // 注册属性
    zend_declare_property(study_coroutine_server_coro_ce_ptr, ZEND_STRL("zsock"), &zsock, ZEND_ACC_PUBLIC);
    zend_declare_property_string(study_coroutine_server_coro_ce_ptr, ZEND_STRL("host"), "", ZEND_ACC_PUBLIC);
    zend_declare_property_long(study_coroutine_server_coro_ce_ptr, ZEND_STRL("port"), -1, ZEND_ACC_PUBLIC);
    zend_declare_property_long(study_coroutine_server_coro_ce_ptr, ZEND_STRL("errCode"), 0, ZEND_ACC_PUBLIC);
    zend_declare_property_string(study_coroutine_server_coro_ce_ptr, ZEND_STRL("errMsg"), "", ZEND_ACC_PUBLIC);
}