#include "study_coroutine.h"
using Study::PHPCoroutine;



long PHPCoroutine::create(zend_fcall_info_cache *fci_cache,uint32_t argc, zval *argv)
{
    php_coro_args php_coro_args;
    php_coro_args.fci_cache = fci_cache;
    php_coro_args.argc = argc;
    php_coro_args.argv = argv;
    
    save_task(get_task());

    return 0; //这里本应该返回协成的id 但是我们还没有到这一步，所以先返回0
}
//保存当前协成的栈帧
void PHPCoroutine::save_task(php_coro_task *task)
{
     save_vm_stack(task);
}

//保存数据
void PHPCoroutine::save_vm_stack(php_coro_task *task)
{
     task->vm_stack_top = EG(vm_stack_top);
     task->vm_stack_end = EG(vm_stack_end);
     task->vm_stack     = EG(vm_stack);
     task->vm_stack_page_size = EG(vm_stack_page_size);
     task->execute_data = EG(current_execute_data);
}
//暂时先返回一个空指针
php_coro_task* PHPCoroutine::get_task()
{
    return nullptr;
}
