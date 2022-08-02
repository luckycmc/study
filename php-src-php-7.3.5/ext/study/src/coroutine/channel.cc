#include "coroutine.h"
#include "coroutine_channel.h"
#include "timer.h"

using study::Coroutine;
using study::coroutine::Channel;
using study::Timer;
using study::TimerManager;
using study::timer_manager;

//channel 类的构造方法
Channel::Channel(size_t  _capacity):
capacity(_capacity)
{

}
//析构方法
Channel::~Channel()
{

}
//实现协程超时的回调函数
static void sleep_timeout(void* param)
{
    ((Coroutine *) param)->resume();
}
//实现channel 的popo操作
void* Channel::pop(double timeout)
{
     Coroutine *co = Coroutine::get_current(); //获取当前协成对象
     void* data;

     //判断队列是否为空 我们先判断有没有数据，如果没有数据，并且设置了协程超时等待的数据的时间，
     //那么，我们就设置一个定时器。然后，把这个协程加入到消费者等待队列里面，最后，yield这个协程。
     if(data_queue.empty()){
          
             if (timeout > 0)
             {
                  //添加定时器
                  timer_manager.add_timer(timeout * Timer::SECOND, sleep_timeout, (void*)co);
             }
             consumer_queue.push(co); //存放当前的协程对象
             co->yield(); // 协成切换
     }
     //这段代码实际上是专门为timeout准备的。因为协程timeout恢复运行的时候，
     //Channel可能还是没有数据，所以我们需要这一步判断。
     if (data_queue.empty())
     {
         return nullptr;
     }
     //取出Channel里面的数据，然后如果有生产者协程在等待，那么就resume那个生产者协程。
     //最后，等生产者协程执行完毕，或者生产者协程主动yield，才会回到消费者协程，最后返回data。
     data = data_queue.front();
     data_queue.pop();

     /**
      * notice producer
      * 
      */
     if(!producer_queue.empty())
     {
         co = producer_queue.front();
         producer_queue.pop();
         co->resume(); //协成恢复
     }
     return data;
}

//实现pop 操作
bool Channel::push(void* data,double timeout) 
{
    Coroutine *co = Coroutine::get_current();
    if(data_queue.size() == capacity)
    {
       if (timeout > 0)
        {
            timer_manager.add_timer(timeout * Timer::SECOND, sleep_timeout, (void*)co);
        }
        producer_queue.push(co);
        co->yield();
    }
    /**
     * channel full
     */
    if (data_queue.size() == capacity)
    {
        return false;
    }

    data_queue.push(data);
    /**
     * notice consumer
     */
    if (!consumer_queue.empty())
    {
        co = consumer_queue.front();
        consumer_queue.pop();
        co->resume();
    }

    return true;
}

bool Channel::empty()
{
    return data_queue.empty();
}

void* Channel::pop_data()
{
    void *data;

    if (data_queue.size() == 0)
    {
        return nullptr;
    }
    data = data_queue.front();
    data_queue.pop();
    return data;
}
