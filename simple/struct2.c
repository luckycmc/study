#include <stdio.h>
//
struct {
	
	  char  *name;
	  int    num;
	  int    age;
	  char   group;
	  float  score;
	  
}class[] = {

     {"Li ping", 5, 18, 'C', 145.0},
     {"Zhang ping", 4, 19, 'A', 130.5},
     {"He fang", 1, 18, 'A', 148.5},
     {"Cheng ling", 2, 17, 'F', 139.0},
     {"Wang ming", 3, 17, 'B', 144.5}
};

int main()
{
	int i,num_140 = 0;
	float sum =0;
	//统计结构体中的数据
	for(i=0;i<5;i++){
		
		 sum+=class[i].score;//统计总数据 
		 
		 if(class[i].score<140) num_140++;
		 printf("\n");
		 printf(class[i].name);
	} 
	//打印输出数据 
	printf("sum=%.2f\naverage=%.2f\nnum_140=%d\n", sum, sum/5, num_140); 
	return 0; 
}

