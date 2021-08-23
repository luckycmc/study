#include <stdio.h>
/**
 * @brief 
 * 指针所占用的字节数
 * 
 */
const int MAX = 3;

int main()
{
    int var[]= {10,100,200};
    int i;
    for ( i = 0; i < MAX; i++)
    {
       // printf("Value of var[%d]=%d\n", i, var[i]);
    }
    
    /**
     *  定义一个指针数组
     * */
    int *ptr[MAX];
    for ( i = 0; i < MAX; i++)
    {
        ptr[i] = &var[i]; //存放当前元素的地址
        //printf("当前数组的地址%p\n", ptr[i]);
    }

    //通过指针数组来访问数据
    for ( i = 0; i < MAX; i++)
    {
       // printf("Value of var[%d] = %d\n", i, *ptr[i] ); //  *ptr[i] 表示访问当前数据
    }
    /*****************指针的测试 start *****************/
    //指针占用 8个字节在linux  下
    int a=10;
    int *p ;
    p = &a;
    printf("整型指针用了%d个字节\n",sizeof(p));

    float b = 12;
    float *b1;
    b1 = &b;
    printf("浮点型指针用了%d个字节\n",sizeof(b1));

    typedef struct student{
            
              int num;
              char *name;
              float score;
    }student_s;

     student_s *student ;
     student_s stu1;
     student = &stu1;
     printf("结构体指针用了%d个字节\n",sizeof(student));
     /*****************指针的测试 end*****************/
    return 0;
}