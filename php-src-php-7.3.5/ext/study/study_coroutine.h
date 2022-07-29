#ifndef STUDY_COROUTINE_H
#define STUDY_COROUTINE_H

#include "php_study.h"
#include "coroutine.h"
#include <stack>

#define DEFAULT_PHP_STACK_SIZE 8192
#define PHP_CORO_TASK_SLOT ((int)((ZEND_MM_ALIGNED_SIZE(sizeof(php_coro_task)) + ZEND_MM_ALIGNED_SIZE(sizeof(zval)) - 1) / ZEND_MM_ALIGNED_SIZE(sizeof(zval))))
//php_study_fci_fcc是一个包含了zend_fcall_info和zend_fcall_info_cache的结构体
struct php_study_fci_fcc
{
    zend_fcall_info fci;
    zend_fcall_info_cache fcc;
};
//定义协成参数结构体
struct php_coro_args
{
     zend_fcall_info_cache *fci_cache;
     zval *argv;   
     uint32_t argc; //参数个数
     
};
// save the coroutine stack info  保存的协程状态信息结构体php_coro_task 保存PHP的栈
struct php_coro_task
{
    zval *vm_stack_top;        //是协程栈栈顶。
    zval *vm_stack_end;        //是协程栈栈底。  
    zend_vm_stack vm_stack;    //是协程栈指针。 
    size_t vm_stack_page_size;           //是协程栈页大小。
    Study::Coroutine *co;               // 协成对象
    zend_execute_data *execute_data;   //是当前协程栈的栈帧。
    // 引入栈
    std::stack<php_study_fci_fcc *> *defer_tasks;
};


namespace Study
{
class PHPCoroutine
{   
    public:
      static long create(zend_fcall_info_cache *fci_cache, uint32_t argc, zval *argv);
      static void defer(php_study_fci_fcc *defer_fci_fcc);
      static int sleep(double seconds);
    protected:
    
      static php_coro_task main_task;

      static void save_task(php_coro_task *task);  //保存 PHP 栈
      static void save_vm_stack(php_coro_task *task); //保存PHP栈的内容
      static php_coro_task* get_task();   //获取PHP 栈的内容
      static void create_func(void *arg);
      static void vm_stack_init(void); //PHP栈初始化
};
}

#endif    /* STUDY_COROUTINE_H */