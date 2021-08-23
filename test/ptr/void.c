#include <stdio.h>
/*****************指针的测试
 * void 关键字解释
 * 当返回值为空时可以使用void
 * void  empty(int a)
 * 当函数参数为空时，可以使用void
 * void empty(void)
 * 
 * void 指针的作用
 * void 指针:void *
 * 可以用其指代任何指针类型
 * 但是不能用void 指针直接操作只能转换对应的类型指针后才能操作
 * 
 * 空指针的学习和对应指针的转换
 * */
void test(void *a)//可以代表任何类型的指针
{
    printf("void *---->%d\n", *(int *)a); //(int *)a 表示强制转换成 整型指针  最前面*表示输出其对应的值
    printf("void *---->%p\n", (int *)a);   // 和之前的指针一样
}
//主函数
int main()
{
        enum Season {
             Spring, Summer = 18, Autumn, Winter
        } season;
        enum Season *s = &season;
        void* vs = s; //空指针类型赋值
        //printf("vs= %d\n", vs); //error  不能直接用于操作
        int* ns = (int*)vs; //转换成对应的 int型指针
        printf("ns= %d\n", *ns);
        printf("**************************\n");
        int m = 10;
        int *p = &m;
        // 输出对应的指针
        printf("void *---->%p\n",p);
        test(p);
        return 0;
}