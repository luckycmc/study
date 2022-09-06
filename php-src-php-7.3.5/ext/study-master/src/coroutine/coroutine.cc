#include "coroutine.h"
#include "timer.h"

using study::Coroutine;

Coroutine* Coroutine::current = nullptr;

long Coroutine::last_cid = 0;
//map容器
std::unordered_map<long, Coroutine*> Coroutine::coroutines;
size_t Coroutine::stack_size = DEFAULT_C_STACK_SIZE;
/***PHP 栈的回调函数**/
st_coro_on_swap_t Coroutine::on_yield = nullptr;
st_coro_on_swap_t Coroutine::on_resume = nullptr;
st_coro_on_swap_t Coroutine::on_close = nullptr;

void* Coroutine::get_current_task()
{
    return current ? current->get_task() : nullptr;
}

void* Coroutine::get_task()
{
    return task;
}

Coroutine* Coroutine::get_current()
{
    return current;
}
// 设置当前的栈
void Coroutine::set_task(void *_task)
{
    task = _task;
}
// 创建协成
long Coroutine::create(coroutine_func_t fn, void* args)
{
    return (new Coroutine(fn, args))->run();
}
// 协成切换
void Coroutine::yield()
{
    assert(current == this);
    on_yield(task);  // 切换PHP栈
    current = origin;
    ctx.swap_out();
}
// 协成的恢复h
void Coroutine::resume()
{
    assert(current != this);
    //恢复PHP栈
    on_resume(task);
    //切换栈
    origin = current;
    current = this;
    ctx.swap_in();
    if (ctx.is_end())
    {
        assert(current == this);
        on_close(task);
        current = origin;
        coroutines.erase(cid);
        delete this;
    }
}
// 超时设置
static void sleep_timeout(void *param)
{
    ((Coroutine *) param)->resume();
}
// 模拟阻塞恢复 
int Coroutine::sleep(double seconds)
{
    Coroutine* co = Coroutine::get_current();

    timer_manager.add_timer(seconds * Timer::SECOND, sleep_timeout, (void*)co);
   
    co->yield();
    return 0;
}
/**PHP 栈回调函数的设置 start**/
void Coroutine::set_on_yield(st_coro_on_swap_t func)
{
    on_yield = func;
}

void Coroutine::set_on_resume(st_coro_on_swap_t func)
{
    on_resume = func;
}

void Coroutine::set_on_close(st_coro_on_swap_t func)
{
    on_close = func;
}
/**PHP 栈回调函数的设置 start**/