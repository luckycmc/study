# include<stdio.h>

int main()
{
   
   char *p;
   
   char s[] = "mjwefg456";
   
   //指针指向字符串的首字母
   p = s;
   
   for(;*p!='\0';p++){
   	  
   	  printf("%c\n",*p);
   	
   } 
} 
