#ifndef STUDY_COROUTINE_SERVER_H
#define STUDY_COROUTINE_SERVER_H

#include "php_study.h"
#include "study_coroutine.h"
#include "socket.h"
#include "coroutine_socket.h"
#include "error.h"

namespace study { namespace phpcoroutine {
class Server
{
private:
    study::coroutine::Socket *socket = nullptr;
    php_study_fci_fcc *handler = nullptr;
    bool running = false;

public:
    Server(char *host, int port); // socket 类构造方法
    ~Server(); //析构方法
    bool start();  //启动
    bool shutdown(); // 关闭
    void set_handler(php_study_fci_fcc *_handler); //设置对应的函数
    php_study_fci_fcc* get_handler();
};
}
}

#endif /* STUDY_COROUTINE_SERVER_H */