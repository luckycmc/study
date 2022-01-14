#include "coroutine.h"

using Study::Coroutine;

Coroutine* Coroutine::current = nullptr;

void* Coroutine::get_current_task()
{
    return Coroutine::current ? Coroutine::current->get_task() : nullptr;
}

void* Coroutine::get_task()
{
    return task;
}

long Coroutine::create(coroutine_func_t fn, void* args)
{
    return (new Coroutine(fn, args))->run();
}