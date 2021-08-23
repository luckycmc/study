#include "./include/swoole.h"
#include "./include/Server.h"


static int php_swoole_onReceive(swFactory *, swEventData *);
static void php_swoole_onStart(swServer *);
static void php_swoole_onShutdown(swServer *);
static void php_swoole_onConnect(swServer *, int fd, int from_id);
static void php_swoole_onClose(swServer *, int fd, int from_id);
static void php_swoole_onTimer(swServer *serv, int interval);
static void sw_destory_server();


