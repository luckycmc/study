#include <stdio.h>
#include <stdlib.h>

enum{
    
     INVALID_COLOR = 0,
     RED = 1,
     GREEN = 2,
};

//定影bird 结构体
struct Bird
{
     
     char *Name;
     char *Addr;
     int Color;
     int  Weight;
     //函数指针
     void (*SetName)(struct Bird *Bird,char *Name);
     void (*SetAddr)(struct Bird *Bird, char *Addr);
     void (*SetColor)(struct Bird *Bird, const int Color);
     void (*SetWeight)(struct Bird *Bird, const int Weight);

     char *(*GetName)(struct Bird *Bird);
     int  (*getColor)(struct Bird *Bird);
};

/*************开发对应的回调函数 start**************/

void SetBirdName(struct Bird *Bird,const char *Name)
{
      if(Bird == NULL){
            return ;
      }
      Bird->Name = Name;
}



/*************开发对应的回调函数 end**************/