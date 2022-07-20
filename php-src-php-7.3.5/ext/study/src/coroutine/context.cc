#include "context.h"
#include "study.h"

using namespace Study;
//协成上下文 构造方法执行
Context::Context(size_t stack_size, coroutine_func_t fn, void* private_data) :
        fn_(fn), stack_size_(stack_size), private_data_(private_data)
{
    swap_ctx_ = nullptr;
    stack_ = (char*) malloc(stack_size_); //栈的大小
    void* sp = (void*) ((char*) stack_ + stack_size_); //sp指针 void *private_data_;
    //栈指针和栈的大小 以及栈的 入口地址
    ctx_ = make_fcontext(sp, stack_size_, (void (*)(intptr_t))&context_func); //设置当前协成的上下文
}

/**
 *  执行协成函数
 * @param arg 
 */
void Context::context_func(void *arg)
{    
     /*
       可以看到，这段代码就是去调用fn_，也就是PHPCoroutine::create_func，
       并且给它传递参数private_data_，也就是php_coro_args *。
      */
     Context *_this = (Context *) arg;
     _this->fn_(_this->private_data_);
     _this->end_ = true;//协程开始运行
     _this->swap_out();
}
/**
 * 让出当前协程的上下文
 * 
 * @return true 
 * @return false 
 */
bool Context::swap_out()
{
    jump_fcontext(&ctx_,swap_ctx_,(intptr_t) this,true);
    return true;
}
/**
 * 
 * 加载要运行的协程
 * @return true 
 * @return false 
 */
bool Context::swap_in()
{
    jump_fcontext(&swap_ctx_, ctx_, (intptr_t) this, true);
    return true;
}
