#include "study_coroutine.h"
#include "coroutine.h"

using Study::PHPCoroutine;
using Study::Coroutine;

php_coro_task PHPCoroutine::main_task = {0};
//协成创建入口
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
// PHP的栈 保存当前的PHP 栈帧,先获取在保存
void PHPCoroutine::save_vm_stack(php_coro_task *task)
{
     
      task->vm_stack_top = EG(vm_stack_top);
      task->vm_stack_end = EG(vm_stack_end);
      task->vm_stack = EG(vm_stack);
      task->vm_stack_page_size = EG(vm_stack_page_size);
      task->execute_data = EG(current_execute_data);
}


//获取当前PHP 的协程栈
php_coro_task* PHPCoroutine::get_task()
{   
    php_coro_task *task = (php_coro_task*)Coroutine::get_current_task();

    return task?task:&main_task; //当前协成存在则返回当前协成否则返回 主协成
}
//协程函数的创建 PHPCoroutine::create_func是用来创建协程任务的 我们可以了解为这是一个辅助函数，辅助我们去创建协程
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

    save_vm_stack(task); //保存当前的PHP栈帧

    task->co = Coroutine::get_current();//获取当前的协成
    task->co->set_task((void *) task);
     //defer
    task->defer_tasks = nullptr;
    //判断是否是用户函数
    if (func->type == ZEND_USER_FUNCTION)
    {
        ZVAL_UNDEF(retval);
        EG(current_execute_data) = NULL;
        zend_init_func_execute_data(call, &func->op_array, retval);
        zend_execute_ex(EG(current_execute_data));
    }
    /****************defer 函数接口 start**********************/
    task = get_task(); //获取当前栈
    std::stack<php_study_fci_fcc*> *defer_tasks = task->defer_tasks;
    // defer 栈弹出
    if (defer_tasks) {
        php_study_fci_fcc *defer_fci_fcc;
        zval result;
        while(!defer_tasks->empty())
        {
            defer_fci_fcc = defer_tasks->top();
            defer_tasks->pop();
            defer_fci_fcc->fci.retval = &result;

            if (zend_call_function(&defer_fci_fcc->fci, &defer_fci_fcc->fcc) != SUCCESS)
            {
                php_error_docref(NULL, E_WARNING, "defer execute error");
                return;
            }
            efree(defer_fci_fcc);
        }
        delete defer_tasks;
        task->defer_tasks = nullptr;
    }
    /****************defer 函数接口 end**********************/
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
//协成defer 的实现
void PHPCoroutine::defer(php_study_fci_fcc *defer_fci_fcc)
{
    php_coro_task *task = (php_coro_task *)get_task();
    if (task->defer_tasks == nullptr)
    {
        task->defer_tasks = new std::stack<php_study_fci_fcc *>;
    }
    task->defer_tasks->push(defer_fci_fcc);
}
//实现sleep 模拟IO阻塞
int PHPCoroutine::sleep()
{
    std::cout << seconds << endl;
    return 0;
}