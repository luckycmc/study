#include "coroutine.h"

using Study::Coroutine;


Coroutine* Coroutine::current = nullptr; //默认当前栈帧为空指针

long Coroutine::last_cid = 0;   //初始化协成的id0
size_t Coroutine::stack_size = DEFAULT_C_STACK_SIZE; //栈的大小
// 设置map 模板
std::unordered_map<long, Coroutine*> Coroutine::coroutines;
/**协成切换的指针函数**/

/*********coroutine/coroutine.cc 有问题********/   

void* Coroutine::get_current_task()
{
    return current ? current->get_task() : nullptr;
}
// 获取当前PHP 栈
void* Coroutine::get_task()
{
    return task;
}
// 获取当前协成
Coroutine* Coroutine::get_current()
{   

   return current;
}

void Coroutine::set_task(void *_task)
{
    task = _task; //设置协成
}
//让出当前协成
void Coroutine::yield()
{
   current = origin;
   ctx.swap_out();
}
//创建协成 协成类
long Coroutine::create(coroutine_func_t fn, void* args)
{   
    return (new Coroutine(fn, args))->run();//执行对应的协成函数
}

//模拟阻塞IO
int Coroutine::sleep(double seconds)
{
    return 0;
}

