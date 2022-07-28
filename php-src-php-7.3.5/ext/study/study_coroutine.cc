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
    
    save_task(get_task());//保存当前的栈空间

    return Coroutine::create(create_func,(void *) &php_coro_args);  // 1.直接进到 Context.h 的构造方法中
    //return 0; //这里本应该返回协成的id 但是我们还没有到这一步，所以先返回0
}
//保存当前协成的栈帧
void PHPCoroutine::save_task(php_coro_task *task)
{
     save_vm_stack(task);
}

//保存当前栈的数据信息 zend_vm 也是有想用的堆栈信息 
// 通过全局函数EG获取对应的信息 EG 是PHP 提供的
void PHPCoroutine::save_vm_stack(php_coro_task *task)
{
     task->vm_stack_top = EG(vm_stack_top);    //通过EG全局变量来获取当前栈的信息
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
    //这个方法的目的是初始化一个新的PHP栈，因为我们即将要创建一个协程了
    vm_stack_init(); // get a new php stack
    call = (zend_execute_data *) (EG(vm_stack_top));
    task = (php_coro_task *) EG(vm_stack_top);
    EG(vm_stack_top) = (zval *) ((char *) call + PHP_CORO_TASK_SLOT * sizeof(zval));
    //将要执行的数据入栈
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
    //符号表
    call->symbol_table = NULL;
    //初始化call这个zend_execute_data之后，我们把它赋值给EG(current_execute_data)
    EG(current_execute_data) = call;
    //把当前的协程栈信息保存在task里面。
    save_vm_stack(task);

    task->co = Coroutine::get_current();
    task->co->set_task((void *) task);

    if (func->type == ZEND_USER_FUNCTION)
    {
        ZVAL_UNDEF(retval);
        EG(current_execute_data) = NULL;
        //zend_init_func_execute_data的作用是去初始化zend_execute_data
        zend_init_func_execute_data(call, &func->op_array, retval); //初始化执行栈
        //zend_execute_ex的作用就是去循环执行executor_globals.current_execute_data指向的opline。
        //此时，这些opline就是我们用户空间传递的函数。
        zend_execute_ex(EG(current_execute_data));
    }

    zval_ptr_dtor(retval);
}

//申请一个新的PHP 栈
void PHPCoroutine::vm_stack_init(void)
{
    uint32_t size = DEFAULT_PHP_STACK_SIZE; // 8M
    zend_vm_stack page = (zend_vm_stack) emalloc(size);//分配一块栈空间
    //栈顶和栈的底部page->top的作用是指向目前的栈顶，这个top会随着栈里面的数据而不断的变化。
    //压栈，top往靠近end的方向移动个；出栈，top往远离end的方向移动。
    //page->end的作用就是用来标识PHP栈的边界，以防'栈溢出'。这个page->end可以作为是否要扩展PHP栈的依据
    page->top = ZEND_VM_STACK_ELEMENTS(page);
    page->end = (zval*) ((char*) page + size);
    page->prev = NULL;  //上一个栈指针
    
    // 当前的栈针信息保存到EG 中 用作当前函数执行  去修改现在的PHP栈，让它指向我们申请出来的新的PHP栈空间
    EG(vm_stack) = page;
    EG(vm_stack)->top++;
    EG(vm_stack_top) = EG(vm_stack)->top;
    EG(vm_stack_end) = EG(vm_stack)->end;
    EG(vm_stack_page_size) = size;

}
