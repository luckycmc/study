#include "context.h"
#include "study.h"
#include "log.h"

using study::Context;
//协成上下文 构造方法执行 
// 在Coroutine的构造函数中完成了协程对象Coroutine的创建与初始化，以及Context对象的创建与初始化（创建了c栈）
Context::Context(size_t stack_size, coroutine_func_t fn, void* private_data) :
        fn_(fn), stack_size_(stack_size), private_data_(private_data)
{     
    swap_ctx_ = nullptr;  //默认空指针

    try
    {
        stack_ = new char[stack_size_];
    }
    catch(const std::bad_alloc& e)
    {
        stError("%s", e.what());
    }
    // sp指向内存最高地址处
    void* sp = (void*) ((char*) stack_ + stack_size_);
    //make_fcontext函数用于创建一个执行上下文 ctx_ 是一个空指针 void *ptr  此时也是 context 的对象
    // 申请对应的空间模拟一个栈的实现 (栈顶,栈的大小,函数的入口地址也就是栈的入口地址)
    // 创建上下文:启动函数+执行栈
    ctx_ = make_fcontext(sp, stack_size_, (void (*)(intptr_t))&context_func);
}

/**
 *  执行协成函数
 * @param arg 
 */
void Context::context_func(void *arg)
{    
     /* 
        问题：参数arg为什么是Context对象呢，是如何传递的呢？这就涉及到jump_fcontext汇编实现，以及jump_fcontext的调用了？
        调用jump_fcontext函数时，第三个参数传递的是this，即当前Context对象；而函数jump_fcontext汇编实现时，
        将第三个参数的内容拷贝到%rdi寄存器中，当协程换入执行函数context_func时，寄存器%rdi存储的就是第一个参数，
        即Context对象。
        /*********函数的执行/
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
//协成结束后释放对应的内存
Context::~Context()
{
    if (swap_ctx_)
    {
       delete[]stack_;
       stack_ = nullptr;
    }
}
