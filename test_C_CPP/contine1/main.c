#include <studio.h>

int stop = 0;
static void foo_5(schedule_t *S, void *ud) {
    while (!stop) {
        co_yield(S);
    }
}

static void test5(schedule_t *S) {
    printf("test5 start===============\n");
    struct timeval begin;
    struct timeval end;
    int i;
    int count = 10000;

    gettimeofday(&begin, NULL);
    for (i = 0; i < count; ++i) {
        co_new(S, foo_5, NULL);
    }
    gettimeofday(&end, NULL);
    printf("create time=%f\n", timediff(&begin, &end));

    gettimeofday(&begin, NULL);
    for (i =0; i < 1000000; ++i) {
        int co = (i % count) + 1;
        co_resume(S, co);
    }
    gettimeofday(&end, NULL);
    printf("swap time=%f\n", timediff(&begin, &end));

    // 先释放掉原来的
    stop = 1;
    for (i = 0; i < count; ++i) {
        int co = (i % count) + 1;
        co_resume(S, co);
    }
    gettimeofday(&begin, NULL);
    for (i = 0; i < count; ++i) {
        co_new(S, foo_5, NULL);
    }
    gettimeofday(&end, NULL);
    printf("create time2=%f\n", timediff(&begin, &end));
    printf("test5 end===============\n");
}

int main()
{
    
    test5(); //测试代码

    return 0;
}