#include "AddressBook.h"
//选着对应的菜单
void Choose()
{
    printf("********1.添加新联系人**********\n");
    printf("********2.展示已有联系人********\n");
    printf("********3.修改联系人信息********\n");
    printf("********4.查找联系人信息********\n");
    printf("********5.删除联系人************\n");
    printf("********6.对已有联系人排序******\n");
    printf("********7.退出通讯录************\n");
}

int main()
{  
    //定义结构体数组
    message arr[num];
    int b =0;
    while(1)
    {
        Choose();
        int i;
        printf("请输入你的选择：\n");
        scanf("%d\n",&i);
        switch(i)
        {
            case ADD :
               AddMessage(arr,&b);
               break;
            case SHOW:
                Show(arr,b);
                break;
            case EXITS:
                exit(0);
                break;
            default :
                printf("你的选择有误，重新选择！\n");
                i = 0;
                break;
        }
    }
    
    return 1;
}