#include "context.h"
#include "study.h"
#include "log.h"
#include <iostream>

using namespace study;
using namespace std;
//协成的构造方法
Context::Context(size_t stack_size, coroutine_func_t fn, void* private_data) :
        fn_(fn), stack_size_(stack_size), private_data_(private_data)
{
    swap_ctx_ = nullptr;

    try
    {
        stack_ = new char[stack_size_];
    }
    catch(const std::bad_alloc& e)
    {
        stError("%s", e.what());
    }

    void* sp = (void*) ((char*) stack_ + stack_size_);  // sp指针
    ctx_ = make_fcontext(sp, stack_size_, (void (*)(intptr_t))&context_func); //创建协成的上下文
}
//析构方法
Context::~Context()
{
    if (swap_ctx_)
    {
        delete[] stack_;
        stack_ = nullptr;
    }
}
//保存当前的上下文
bool Context::swap_in()
{
    jump_fcontext(&swap_ctx_, ctx_, (intptr_t) this, true);
    return true;
}
//让出当前的上下文
bool Context::swap_out()
{
    jump_fcontext(&ctx_, swap_ctx_, (intptr_t) this, true);
    return true;
}
//执行用户的函数
void Context::context_func(void *arg)
{
    Context *_this = (Context *) arg;  //参数
    _this->fn_(_this->private_data_);  //对应用户函数
    _this->end_ = true;
    _this->swap_out(); //协成切换
}