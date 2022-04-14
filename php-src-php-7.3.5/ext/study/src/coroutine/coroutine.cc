
#include "coroutine.h"
using Study::Coroutine;

Coroutine* Coroutine::current = nullptr;

long Coroutine::last_cid = 0; //协成id初始化
std::unordered_map<long, Coroutine*> Coroutine::coroutines;

/*********coroutine/coroutine.cc 有问题********/   

void* Coroutine::get_current_task()
{
    return current ? Coroutine::current->get_task() : nullptr;
}

Coroutine* Coroutine::get_current()
{
    return current;
}
void Coroutine::set_task(void *_task)
{
    task = _task;
}

void* Coroutine::get_task()
{
    return task;
}
//协成的创建
long Coroutine::create(coroutine_func_t fn, void* args)
{
   return (new Coroutine(fn, args))->run();
}