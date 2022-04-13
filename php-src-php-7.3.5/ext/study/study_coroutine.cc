#include "study_coroutine.h"

using Study::PHPCoroutine;

long PHPCoroutine::create(zend_fcall_info_cache *fci_cache, uint32_t argc, zval *argv)
{
    php_coro_args php_coro_args;
    php_coro_args.fci_cache = fci_cache;
    php_coro_args.argv = argv;
    php_coro_args.argc = argc;
    save_task(get_task());

    //return 0;  // 这里本应该是返回创建的协程id，但是我们还没有到这一步，所以先返回0
    return Coroutine::create(create_func,(void *) &php_coro_args);
}
//保存php+c栈
void PHPCoroutine::save_task(php_coro_task *task)
{
    save_vm_stack(task);
}
// PHP的栈
void PHPCoroutine::save_vm_stack(php_coro_task *task)
{
     
      task->vm_stack_top = EG(vm_stack_top);
      task->vm_stack_end = EG(vm_stack_end);
      task->vm_stack = EG(vm_stack);
      task->vm_stack_page_size = EG(vm_stack_page_size);
      task->execute_data = EG(current_execute_data);
}

php_coro_task PHPCoroutin::main_task = {0};
//获取当前PHP 的协程栈
php_coro_task* PHPCoroutine::get_task()
{   
    php_coro_task *task = (php_coro_task*)Coroutine::get_current_task();

    return task?task:nullptr;
}
//协程函数的创建
void PHPCoroutine::create_func(void *arg)
{
    int i;
    php_coro_args *php_arg = (php_coro_args *) arg;
    zend_fcall_info_cache fci_cache = *php_arg->fci_cache;
    zend_function *func = fci_cache.function_handler;
    zval *argv = php_arg->argv;
    int argc = php_arg->argc;
    php_coro_task *task;
    zend_execute_data *call;
    zval _retval, *retval = &_retval;

    vm_stack_init(); // get a new php stack 这个方法的目的是初始化一个新的PHP栈，因为我们即将要创建一个协程了

    call = (zend_execute_data *) (EG(vm_stack_top));
    task = (php_coro_task *) EG(vm_stack_top);
    EG(vm_stack_top) = (zval *) ((char *) call + PHP_CORO_TASK_SLOT * sizeof(zval));

    call = zend_vm_stack_push_call_frame(
        ZEND_CALL_TOP_FUNCTION | ZEND_CALL_ALLOCATED,
        func, argc, fci_cache.called_scope, fci_cache.object
    );

    for (i = 0; i < argc; ++i)
    {
        zval *param;
        zval *arg = &argv[i];
        param = ZEND_CALL_ARG(call, i + 1);
        ZVAL_COPY(param, arg);
    }

    call->symbol_table = NULL;

    EG(current_execute_data) = call;

    save_vm_stack(task);

    task->co = Coroutine::get_current();
    task->co->set_task((void *) task);

    if (func->type == ZEND_USER_FUNCTION)
    {
        ZVAL_UNDEF(retval);
        EG(current_execute_data) = NULL;
        zend_init_func_execute_data(call, &func->op_array, retval);
        zend_execute_ex(EG(current_execute_data));
    }

    zval_ptr_dtor(retval);
}
//得到一个新的PHP 栈
void PHPCoroutine::vm_stack_init(void)
{
    uint32_t size = DEFAULT_PHP_STACK_PAGE_SIZE;
    zend_vm_stack page = (zend_vm_stack) emalloc(size);
    /**
     page->top的作用是指向目前的栈顶，这个top会随着栈里面的数据而不断的变化。
     压栈，top往靠近end的方向移动个；出栈，top往远离end的方向移动。
     page->end的作用就是用来标识PHP栈的边界，以防'栈溢出'。
     这个page->end可以作为是否要扩展PHP栈的依据。
     * 
     */
    page->top = ZEND_VM_STACK_ELEMENTS(page);  //PHP 栈顶
    page->end = (zval *)((char *)page+size);   //php 栈底
    page->prev = NULL;  //上一个栈
    /*这段代码的作用是去修改现在的PHP栈，让它指向我们申请出来的新的PHP栈空间。 start*/
    EG(vm_stack) = page;
    EG(vm_stack)->top++;
    EG(vm_stack_top) = EG(vm_stack)->top;
    EG(vm_stack_end) = EG(vm_stack)->end;
    EG(vm_stack_page_size) = size;
     /*这段代码的作用是去修改现在的PHP栈，让它指向我们申请出来的新的PHP栈空间。 end*/

}