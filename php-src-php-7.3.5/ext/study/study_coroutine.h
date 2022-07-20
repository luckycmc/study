#ifndef STUDY_COROUTINE_H
#define STUDY_COROUTINE_H

#include "php_study.h"

#define DEFAULT_PHP_STACK_SIZE 8192
//定义协成参数结构体
struct php_coro_args
{
     zend_fcall_info_cache *fci_cache;
     zval *argv;   
     uint32_t argc; //参数个数
};
// save the coroutine stack info 
struct php_coro_task
{
    zval *vm_stack_top;        //是协程栈栈顶。
    zval *vm_stack_end;        //是协程栈栈底。  
    zend_vm_stack vm_stack;    //是协程栈指针。 
    size_t vm_stack_page_size;           //是协程栈页大小。
    zend_execute_data *execute_data;   //是当前协程栈的栈帧。
};
namespace Study
{
class PHPCoroutine
{   
    public:
      static long create(zend_fcall_info_cache *fci_cache, uint32_t argc, zval *argv);
    
    protected:
    
      static php_coro_task main_task;

      static void save_task(php_coro_task *task);
      static void save_vm_stack(php_coro_task *task);
      static php_coro_task* get_task();
      static void create_func(void *arg);
      static void vm_stack_init(void); //PHP栈初始化
};
}

#endif    /* STUDY_COROUTINE_H */