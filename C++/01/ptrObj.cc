#include <iostream>
using namespace std;

class Student
{
     public:
         int num;
         int score;
         //声明构造方法
         Student(int n, int s);
         void Print();  //声明输出的函数信息
};

//实习student中的方法
Student::Student(int n, int s)
{
    this->num = n; //也可以使用this 指针这点和PHP 很像
    score = s;
}

void Student::Print()
{
    cout<<num <<"\t" << score <<endl; // 也可以使用this->num; 这点和PHP ；类似
}

int main()
{
    Student stud(1,100);
    Student *p = &stud ;    // *p 是student 类型的指针对象
    //通过指针的方式调用类中的方法和属性
    cout <<"(*p).num:"<<(*p).score <<endl;
    cout <<"p->score:"<<p->score <<endl;
    (*p).Print();
    p->Print();
}