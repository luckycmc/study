#include "coroutine.h"
#include "timer.h"

using study::Coroutine;
/**
1.每个协程都需要管理自己的c栈与php栈；

2.Context封装了c栈的管理操作;ctx_字段保存的是寄存器%rsp的内容（指向c栈栈顶位置）；
swap_ctx_字段保存的是将被换出的协程寄存器%rsp内容（即，将被换出的协程的c栈栈顶位置）；
SwapIn()对应协程换入操作；SwapOut()对应协程换出操作；

3.参考jump_fcontext实现，协程在换出时，会将寄存器%rip，%rbp等暂存在c栈栈顶；
协程在换入时，相应的会从栈顶恢复这些寄存器的内容；
Coroutine管理着协程所有内容；cid字段表示当前协程的ID；task字段指向当前协程的php_coro_task结构，
该结构中保存的是当前协程的php栈信息（vm_stack_top，execute_data等）；ctx字段指向的是当前协程的Context对象；
origin字段指向的是另一个协程Coroutine对象；yield()和resume()对应的是协程的换出换入操作；

4.注意到php_coro_task结构的co字段指向其对应的协程对象Coroutine；

5.Coroutine还有一些静态属性，静态属性的属于类属性，所有协程共享的；last_cid字段存储的是当前最大的协程ID，
创建协程时可用于生成协程ID；current字段指向的是当前正在运行的协程Coroutine对象；on_yield和on_resume是两个函数指针，
用于实现php栈的切换操作，实际指向的是方法PHPCoroutine::on_yield和PHPCoroutine::on_resume；
 * 
 */
// Coroutine管理着协程所有内容；包括 PHP栈和C栈
Coroutine* Coroutine::current = nullptr; //默认当前栈帧为空指针

long Coroutine::last_cid = 0;   //初始化协成的id0
size_t Coroutine::stack_size = DEFAULT_C_STACK_SIZE; //栈的大小
// 设置map 模板
std::unordered_map<long, Coroutine*> Coroutine::coroutines;
/**协成切换的指针函数**/
st_coro_on_swap_t Coroutine::on_yield = nullptr;
st_coro_on_swap_t Coroutine::on_resume = nullptr;
st_coro_on_swap_t Coroutine::on_close = nullptr; // 新增的一行
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
   assert(current == this);
   //切换PHP栈
   on_yield(task);
   current = origin;
   //切换C栈
   ctx.swap_out();
}
//恢复当前协成
/**
 协程之间可以通过Coroutine对象的origin字段形成一个类似链表的结构；Co::yield()换出当前协程时，会换入origin协程；
 在A协程种调用Co::resume()恢复B协程时，会换出A协程，换入B协程，同时标记A协程为B的origin协程；
 * 
 */
void Coroutine::resume()
{   
    assert(current != this);
    on_resume(task);
    origin = current;  //主程序中current为null
    current = this;  //执行当前的协程对象
    ctx.swap_in();   // 切换C栈
    //判断协成是否结束
    if (ctx.is_end())
    {
        //cid = current->get_cid();
        //printf("in resume method: co[%ld] end\n", cid);
        on_close(task); // 新增的一行 释放PHP栈
        current = origin; //上一个协程对象   
        coroutines.erase(cid); //释放map 中的 对象
        delete this;// 删除协程对象
    }
}
//创建协成 协成类
long Coroutine::create(coroutine_func_t fn, void* args)
{    
    //new Coroutine(fn, args) 创建一个协成对象
    return (new Coroutine(fn, args))->run();//执行对应的协成函数
}
//超时恢复
static void sleep_timeout(void *param)
{
    ((Coroutine *) param)->resume();
}

//模拟阻塞IO
int Coroutine::sleep(double seconds)
{
    Coroutine* co = Coroutine::get_current();

    timer_manager.add_timer(seconds * Timer::SECOND, sleep_timeout, (void*)co);
   
    co->yield();
    return 0;
}
/**
  on_yield和on_resume用来保存函数指针。
  实际上就是study::PHPCoroutine::on_yield和study::PHPCoroutine::on_resume
  on_yield和on_resume是两个函数指针，用于实现php栈的切换操作，
  实际指向的是方法PHPCoroutine::on_yield和PHPCoroutine::on_resume；
**/
/**********用于 PHP 栈的切换 start************/
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
/**********用于 PHP 栈的切换 end************/
