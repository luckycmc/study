#include "study_coroutine.h"
#include "coroutine.h"

using study::PHPCoroutine;
using study::Coroutine;
// php 栈和C栈的粘合
php_coro_task PHPCoroutine::main_task = {0}; //主协成 也即是主进程
// 
long PHPCoroutine::create(zend_fcall_info_cache *fci_cache,uint32_t argc, zval *argv)
{
    php_coro_args php_coro_args;  //参数

    php_coro_args.fci_cache = fci_cache;
    php_coro_args.argc = argc;
    php_coro_args.argv = argv;
    
    save_task(get_task());//保存当前php的栈空间
    // create_func 是执行函数的空间
    return Coroutine::create(create_func,(void *) &php_coro_args);  // 1.直接进到 Context.h 的构造方法中
    //return 0; //这里本应该返回协成的id 但是我们还没有到这一步，所以先返回0
}
//保存当前协成的栈帧
void PHPCoroutine::save_task(php_coro_task *task)
{
     save_vm_stack(task);
}

//保存当前栈的数据信息 zend_vm 也是有想用的堆栈信息 当前在执行的PHP的脚本的内容
// 通过全局函数EG获取对应的信息 EG 是PHP 提供的
// EG 中的数据也就是当前虚拟机中正在执行的数据
// 通过全局函数EG获取对应的信息 EG 是PHP 提供的 也就是 PHP虚拟机的堆栈信息 也是响应的脚本指向到那了
void PHPCoroutine::save_vm_stack(php_coro_task *task)
{
     task->vm_stack_top = EG(vm_stack_top);    //通过EG全局变量来获取当前栈的信息 当前的栈顶
     task->vm_stack_end = EG(vm_stack_end);    // 当前PHP的栈底
     task->vm_stack     = EG(vm_stack);  // 当前运行的栈指针
     task->vm_stack_page_size = EG(vm_stack_page_size);  //当前运行栈的大小
     task->execute_data = EG(current_execute_data);    // 当前 PHP 栈的执行 也就是opline opcode 的运行位置
}
//暂时先返回一个空指针
php_coro_task* PHPCoroutine::get_task()
{
    php_coro_task *task = (php_coro_task *) Coroutine::get_current_task();
    return task?task:&main_task;
}
/**
 * /对应的执行函数 对应的协成执行函数 
 *  也就是用户空间的指向的函数 liru go（function(){}）
 * 这个协程函数需要在 PHP的虚拟机上执行
 * @param arg 
 */
void PHPCoroutine::create_func(void *arg)
{
    int i;
    // 用户端传递过来的参数
    php_coro_args *php_arg = (php_coro_args *) arg;
    zend_fcall_info_cache fci_cache = *php_arg->fci_cache;
    // 获取需要指向的函数 也会就是 task test_php 中的 t1.php
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
    // copy 对应的参数
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
    task->defer_tasks = nullptr; //deder 使用的赋值为空
    if (func->type == ZEND_USER_FUNCTION)
    {
        ZVAL_UNDEF(retval);
        EG(current_execute_data) = NULL;
        //zend_init_func_execute_data的作用是去初始化zend_execute_data
        zend_init_func_execute_data(call, &func->op_array, retval); //初始化执行栈
        //zend_execute_ex的作用就是去循环执行executor_globals.current_execute_data指向的opline。
        //此时，这些opline就是我们用户空间传递的函数。执行PHP代码 编译后的opcode 
        zend_execute_ex(EG(current_execute_data));
    }
    //判断是都有defer 函数注册有注入则实现函数调用 start************************************/
    task = get_task();
    std::stack<php_study_fci_fcc*> *defer_tasks = task->defer_tasks;

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
     //判断是都有defer 函数注册有注入则实现函数调用  end**/
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
    page->top = ZEND_VM_STACK_ELEMENTS(page);   //栈顶 
    page->end = (zval*) ((char*) page + size);
    page->prev = NULL;  //上一个栈指针
    
    // 当前的栈针信息保存到EG 中 用作当前函数执行  去修改现在的PHP栈，让它指向我们申请出来的新的PHP栈空间
    EG(vm_stack) = page;  //整个栈  也就是 zend_vm_stack
    EG(vm_stack)->top++;
    /*当前栈的栈顶和栈底使用我们新的栈空间*/
    EG(vm_stack_top) = EG(vm_stack)->top; //栈顶 EG(vm_stack) 是当前的page 新栈的栈顶
    EG(vm_stack_end) = EG(vm_stack)->end; // 
    EG(vm_stack_page_size) = size; // 运行栈的大小

}
//实现defer
void PHPCoroutine::defer(php_study_fci_fcc *defer_fci_fcc)
{
    php_coro_task *task = (php_coro_task *)get_task();
    if (task->defer_tasks == nullptr)
    {
        task->defer_tasks = new std::stack<php_study_fci_fcc *>;
    }
    task->defer_tasks->push(defer_fci_fcc);
}
//模拟阻塞
int PHPCoroutine::sleep(double seconds)
{
     Coroutine::sleep(seconds);
     return 0;
}

//PHP 栈的切换 挂起当前协成
void PHPCoroutine::on_yield(void *arg)
{
    php_coro_task *task = (php_coro_task *) arg;
    php_coro_task *origin_task = get_origin_task(task);
    //保存当前的PHP栈
    save_task(task);
    //恢复准备好的上一个PHP栈
    restore_task(origin_task);
}
//php 栈的恢复 恢复当前协成 主要作用是 保存挡当前的PHP栈
// 恢复当前要运行的PHP栈
void PHPCoroutine::on_resume(void *arg)
{
    php_coro_task *task = (php_coro_task *) arg;
    php_coro_task *current_task = get_task();
    //保存当前的栈
    save_task(current_task);
    // 恢复要运行的PHP栈
    restore_task(task);
}
/**
 * load PHP stack
 */
void PHPCoroutine::restore_task(php_coro_task *task)
{
    restore_vm_stack(task);
}
/**
   restore_vm_stack用来重新加载PHP栈。
   之前的PHP栈 执行到到那个地方  就会从那个地方恢复过去 是PHP在虚拟机栈的恢复
   C栈是在cpu 寄存器上的恢复在cpu上的运行
 * load PHP stack 加载当前的PHP执行栈 执行 在虚拟机上执行
 */
inline void PHPCoroutine::restore_vm_stack(php_coro_task *task)
{
    EG(vm_stack_top) = task->vm_stack_top; // 恢复虚拟机的头部
    EG(vm_stack_end) = task->vm_stack_end;  // 恢复虚拟机的结束位置
    EG(vm_stack) = task->vm_stack;           //
    EG(vm_stack_page_size) = task->vm_stack_page_size; //虚拟机的大小
    //指令在虚拟接的执行位置 也要做相应的恢复 也就是说当前的指令在虚拟机上执行到那个位置了
    EG(current_execute_data) = task->execute_data;
}
//释放协成栈
void PHPCoroutine::on_close(void *arg)
{
    php_coro_task *task = (php_coro_task *) arg;
    php_coro_task *origin_task = get_origin_task(task);
    zend_vm_stack stack = EG(vm_stack);
    //php_printf("%p\n", stack);
    efree(stack);  //释放整个运行的PHP 栈
    restore_task(origin_task); //恢复上一个PHP的栈帧
}

// PHPCoroutine::init就是去设置保存、加载PHP栈的回调函数。
void PHPCoroutine::init()
{
    Coroutine::set_on_yield(on_yield);
    Coroutine::set_on_resume(on_resume);
    Coroutine::set_on_close(on_close);
}