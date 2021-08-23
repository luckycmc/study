#include <stdio.h>
#include <stdlib.h>

#define TOTAL 4
/**
  结构体的各个成员会占用不同的内存，互相之间没有影响；
  而共用体的所有成员占用同一段内存，修改一个成员会影响其余所有成员
*/ 
struct{
	char name[20];
	int  num;
	char sex;
	char profession;
	//公用体 
	union{
		 
		  float score;
		  char  course[20]; 
	}sc;
	
}bodys[TOTAL];

int main(){
	
	int i;
	//输入人员信息
	for(i=0;i<TOTAL;i++){
		
		printf("Input info:");
		scanf("%s %d %c %c", bodys[i].name, &(bodys[i].num), &(bodys[i].sex), &(bodys[i].profession));
        if(bodys[i].profession == 's'){  //如果是学生
            scanf("%f", &bodys[i].sc.score);
        }else{  //如果是老师
            scanf("%s", bodys[i].sc.course);
        }
        fflush(stdin); //刷轻屏幕缓存区 
	} 
	
	//输出人员信息
    printf("\nName\t\tNum\tSex\tProfession\tScore / Course\n");
    
    for(i=0; i<TOTAL; i++){
    	
        if(bodys[i].profession == 's'){  //如果是学生
            printf("%s\t%d\t%c\t%c\t\t%f\n", bodys[i].name, bodys[i].num, bodys[i].sex, bodys[i].profession, bodys[i].sc.score);
        }else{  //如果是老师
            printf("%s\t%d\t%c\t%c\t\t%s\n", bodys[i].name, bodys[i].num, bodys[i].sex, bodys[i].profession, bodys[i].sc.course);
        }
        
    }
    return 0;
}
