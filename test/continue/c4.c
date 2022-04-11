#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include <unistd.h>

/**
  但是如何能实现由用户态自己去调度切换呢,  实际上就是在我们每次调用中断的时候，
  自动保存当前执行函数的调用栈上下文和跳转地址，以便在下次执行的时候自动恢复到上次执行点，
  那如果要从底层理解的话就是在不断切换/赋值寄存器 RSP RIP 两个指令的地址,  
  最终由这两个指令最终去完成函数调用栈的恢复和执行
*/

//注册需要调度的函数
//static void task((void *)());

//中断 保存上下文
//static void yield();

//开始运行并调度管理注册的函数

//static int run();

#define RUN_START 1
#define RUN_END   0

static int  stack_size = 1024;

static int  pid = 1;

//定义一个指针函数

typedef void (*funType)(void);

//定义一个上下文环境结构体

typedef struct env{
    
      //指针函数
      funType func;
      ucontext_t ucon;  //协成上下文结构体
      struct env *next,*prev;  //前置和后置节点
      int task_id ;//协成id
      int run_status;  //协成的运行状态

}env;

//当前 链表中的协成

static env *current_list = NULL;

static ucontext_t run_env;

//执行协成

static void  exec()
{
     current_list->func(); //执行对应的回调函数
     current_list->run_status = RUN_END;
     //设置当前 上下文
     setcontext(&run_env);
}

//执行具体的任务函数

static void task(funType func){
     
       env *node = (env *)malloc(sizeof(env)); //创建指向空间

       node->task_id = pid++; //任务id+1
       node->func    = func; //执行对应的回调函数
       node->run_status = RUN_START ;// 执行 状态

       //如果当前节点存在 ,生成链表 双向链表的节点
       if(current_list){
            
             current_list->prev->next = node;
             node->next = current_list;
             node->prev = current_list->prev;
             current_list->prev = node;

       }else{  //空节点处理
            
             node->next = node;
             node->prev = node;
             current_list = node;
       }
       
       //获取当前节点的上下文
       getcontext(&node->ucon);
       //设置当前节点的 栈和空间大小
       node->ucon.uc_stack.ss_sp = malloc(stack_size);
       node->ucon.uc_stack.ss_size = stack_size;
       //建立一个新的上下文
       makecontext(&node->ucon,exec,0); 
}
//执行具体的协成 以链表的方式执行和存放
static int run()
{
     //设置当前协成的辅助节点
     struct env *current_node = current_list;
     //执行对应的操作
     while(current_list)
     {
          printf("task_id=%d--- run_status=%d \r\n",current_list->task_id,current_list->run_status);
          //切换协成上下文
          swapcontext(&run_env,&current_list);
          printf("task_id=%d--- run_status=%d \r\n",current_list->task_id,current_list->run_status);
          sleep(1); //模拟IO等待
          //如果函数调用栈执行完毕  则释放
          if(current_list->run_status == RUN_END){
                
                 current_node = current_list;
                 if(current_node == current_list->next){
                      
                       current_list = NULL;
                 }else{
                       //双向链表节点后移
                       current_list->prev->next = current_list->next;
                       current_list->next->prev = current_list->prev;
                       current_list             = current_list->next;
                 }
                 //释放对应的堆栈
                 free(current_node->ucon.uc_stack.ss_sp);
                 free("context_t中指向的uc_link");
          }else{
              
                current_list = current_list->next; //指针节点后移
          }
     }
}
//协成切换
static void yield()
{
    swapcontext(&current_list,&run_env);
}
//任务1
int fun1()
{
    printf("fun1-a\r\n");
    yield();
    printf("fun1-a\b\r\n");
    return 0;
}

//任务2
void fun2(){
     printf("fun2-\b\r\n");
}
//任务3
void fun3(){
     printf("fun3-\b\r\n");
}
// 主函数

int  main()
{
     task(void(*)(void)fun1);
     task(fun2);
     task(fun3);
     run();
}