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

// save the coroutine stack info
struct php_coro_task
{
    zval *vm_stack_top;           // coroutine stack top 协成栈顶
    zval *vm_stack_end;           // coroutine stack end 站低
    zend_vm_stack vm_stack;       // coroutine stack 是协程栈指针。
    size_t vm_stack_page_size;       //栈的大小
    zend_execute_data *execute_data;  // current coroutine stack frame 是当前协程栈的栈帧。
};

namespace Study
{
    class PHPCoroutine
    {
           public: 
                  static long create(zend_fcall_info_cache *fci_cache,uint32_t argc,zval *argv);
           protected:
                   static void save_task(php_coro_task *task);//当前的PHP栈
                   static void save_vm_stack(php_coro_task *task);
                   static php_coro_task* get_task(); //获取当前的协程栈
    }; 
}

#endif    /* STUDY_COROUTINE_H */