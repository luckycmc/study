#include "coroutine.h"

using Study::Coroutine;

Coroutine* Coroutine::current = nullptr;

long Coroutine::last_cid = 0;

void* Coroutine::get_current_task()
{
    return Coroutine::current ? Coroutine::current->get_task() : nullptr;
}

void* Coroutine::get_task()
{
    return task;
}
//协成的创建
long Coroutine::create(coroutine_func_t fn, void* args = nullptr)
{
    return Coroutine::create(fn, args)->run();
}