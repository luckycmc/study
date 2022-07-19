#include "study_coroutine.h"
using Study::PHPCoroutine;
using Study::Coroutine;

php_coro_task PHPCoroutine::main_task = {0}; //主协成 也即是主进程

long PHPCoroutine::create(zend_fcall_info_cache *fci_cache,uint32_t argc, zval *argv)
{
    php_coro_args php_coro_args;
    php_coro_args.fci_cache = fci_cache;
    php_coro_args.argc = argc;
    php_coro_args.argv = argv;
    
    save_task(get_task());

    return Coroutine::create(create_func,(void *) &php_coro_args);
    //return 0; //这里本应该返回协成的id 但是我们还没有到这一步，所以先返回0
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
    php_coro_task *task = (php_coro_task *) Coroutine::get_current_task();
    return task?task:&main_task;
}
//对应的执行函数
void PHPCoroutine::create_func()
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
    //这个方法的目的是初始化一个新的PHP栈，因为我们即将要创建一个协程了
    vm_stack_init(); // get a new php stack
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

//申请一个新的PHP 栈
void PHPCoroutine::vm_stack_init(void)
{
    uint32_t size = DEFAULT_PHP_STACK_SIZE;
    zend_vm_stack page = (zend_vm_stack) emalloc(size);
    //栈顶和栈的底部
    page->top = ZEND_VM_STACK_ELEMENTS(page);
    page->end = (zval*) ((char*) page + size);
    page->prev = NULL;  //上一个栈指针
    
    // 吧当前的栈针信息保存到EG 中
    EG(vm_stack) = page;
    EG(vm_stack)->top++;
    EG(vm_stack_top) = EG(vm_stack)->top;
    EG(vm_stack_end) = EG(vm_stack)->end;
    EG(vm_stack_page_size) = size;

}
