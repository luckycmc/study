
#ifndef STUDY_COROUTINE_H
#define STUDY_COROUTINE_H

#include "php_study.h"

// 定义一个协程参数结构体
struct php_coro_args
{
     zend_fcall_info_cache *fci_cache;
     zval *argv;
     uint32_t argc;
};

namespace Study
{
    class PHPCoroutine
    {
           
           static long create(zend_fcall_info_cache *fci_cache,uint32_t argc,zval *argv);
    };
}

#endif    /* STUDY_COROUTINE_H */