#include "context.h"
#include "study.h"
#include "log.h"

using study::Context;
//参考jump_fcontext实现，协程在换出时，会将寄存器%rip，%rbp等暂存在c栈栈顶；协程在换入时，相应的会从栈顶恢复这些寄存器的内容；
//协成上下文 构造方法执行  C栈的创建和上下文管理
// 在Coroutine的构造函数中完成了协程对象Coroutine的创建与初始化，以及Context对象的创建与初始化（创建了c栈）
//参数fn是一个函数指针，指向该执行上下文的入口函数；
Context::Context(size_t stack_size, coroutine_func_t fn, void* private_data) :
        fn_(fn), stack_size_(stack_size), private_data_(private_data)
{     
    swap_ctx_ = nullptr;  //默认空指针

    try
    {
        stack_ = new char[stack_size_]; // 申请一块栈的大小 // 分配一块内存储存c栈, 默认2M
    }
    catch(const std::bad_alloc& e)
    {
        stError("%s", e.what());
    }
    // sp指向内存最高地址处
    void* sp = (void*) ((char*) stack_ + stack_size_); //计算出内存的高处地址
    //make_fcontext函数用于创建一个执行上下文 ctx_ 是一个空指针 void *ptr  此时也是 context 的对象
    // 申请对应的空间模拟一个栈的实现 (栈顶,栈的大小,函数的入口地址也就是栈的入口地址)
    // 创建上下文:启动函数+执行栈 在堆中分配一块内存作为该执行上下文的c栈
    // 代码是设置这个最底层的协程的上下文ctx_，比如栈地址，栈大小，协程的入口函数context_func
    ctx_ = make_fcontext(sp, stack_size_, (void (*)(intptr_t))&context_func); // 构建C栈的上下文
}

/**
 *  执行上下文的入口函数为Context::context_func() 
 * 例如在这个函数打断点 b study::Context::context_func() 
 * @param arg 
 */
void Context::context_func(void *arg)
{    
     /* 
        问题：参数arg为什么是Context对象呢，是如何传递的呢？这就涉及到jump_fcontext汇编实现，以及jump_fcontext的调用了？
        调用jump_fcontext函数时，第三个参数传递的是this，即当前Context对象；而函数jump_fcontext汇编实现时，
        将第三个参数的内容拷贝到%rdi寄存器中，当协程换入执行函数context_func时，寄存器%rdi存储的就是第一个参数，
        即Context对象。
        /函数的执行/
       可以看到，这段代码就是去调用fn_，也就是PHPCoroutine::create_func，
       并且给它传递参数private_data_，也就是php_coro_args *。
      */
     Context *_this = (Context *) arg; // 也就是contex的对应
     _this->fn_(_this->private_data_); //用户空间传递过来的参数和函数名 需要指向的函数任务 也就是PHPCoroutine::create_func返回
     _this->end_ = true; //协程运行结束 表示协成运行结束
     _this->swap_out(); //// 切换回main c栈
}
/**
 * 让出当前协程的上下文
 * swap_out的作用是让出当前协程的上下文，去加载其他协程的上下文。
 * 就是当我们跑完了这个协程，需要恢复其他的协程的上下文，让其他的协程继续运行
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
//协成结束后释放对应的C栈内存
Context::~Context()
{
    if (swap_ctx_)
    {
       delete[]stack_;
       stack_ = nullptr;
    }
}
