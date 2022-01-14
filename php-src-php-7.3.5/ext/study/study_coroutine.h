#ifndef STUDY_COROUTINE_H
#define STUDY_COROUTINE_H
#include "php_study.h"
#include "coroutine.h"
#define DEFAULT_PHP_STACK_PAGE_SIZE       8192
#define PHP_CORO_TASK_SLOT ((int)((ZEND_MM_ALIGNED_SIZE(sizeof(php_coro_task)) + ZEND_MM_ALIGNED_SIZE(sizeof(zval)) - 1) / ZEND_MM_ALIGNED_SIZE(sizeof(zval))))

//协程参数结构体
struct php_coro_args
{
    zend_fcall_info_cache *fci_cache;
    zval *argv;
    uint32_t argc;
};
// save the coroutine stack info
struct php_coro_task
{
    zval *vm_stack_top; // coroutine stack top
    zval *vm_stack_end; // coroutine stack end
    zend_vm_stack vm_stack; // current coroutine stack pointer
    size_t vm_stack_page_size;
    zend_execute_data *execute_data; // current coroutine stack frame
    Study::Coroutine *co;
};

namespace Study
{
    class PHPCoroutine 
    {   public:
              static long create(zend_fcall_info_cache *fci_cache, uint32_t argc, zval *argv);

        protected:
                static php_coro_task main_task; //定义一个主协成
                static void save_task(php_coro_task *task);
                static void save_vm_stack(php_coro_task *task);
                static php_coro_task* get_task();
                static void create_func(void *arg); //执行对应的函数
                static void vm_stack_init(void);
    };
}

#endif    /* STUDY_COROUTINE_H */