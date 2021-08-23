#include <stdio.h>
#include <stdlib.h>
//链表中的节点结构
typedef struct lineStack
{
    int data;
    struct lineStack *next;
}lineStack;

// stack 为当前的栈链 a表示入栈的元素

lineStack *push(lineStack *stack,int a)
{
     
      //创建新元素节点
      lineStack *line = (lineStack *)malloc(sizeof(lineStack));
      line->data = a;
      //新节点与头节点建立逻辑关系
      line->next = stack;

      //更新头节点的指针
      stack = line;  //也就是头节点指针往后移动
      printf("stack*************************%p\n",stack);
      return stack;
}

//元素出栈
lineStack *pop(lineStack *stack)
{
     if (stack) {

        lineStack * p=stack;

        stack = stack->next;
        printf("弹栈元素：%d ",p->data);
        if (stack) {
            printf("栈顶元素：%d\n",stack->data);
        }else{
            printf("栈已空\n");
        }
        free(p);
    }else{
        printf("栈内没有元素");
        return stack;
    }
    return stack;
}
//主函数 执行
int main()
{
    lineStack * stack=NULL;
    stack=push(stack, 1);
    stack=push(stack, 2);
    stack=push(stack, 3);
    stack=push(stack, 4);
    stack=pop(stack);
    stack=pop(stack);
    stack=pop(stack);
    stack=pop(stack);
    stack=pop(stack);
    return 0;
}