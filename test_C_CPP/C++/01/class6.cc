#include <iostream>
using namespace std;

class Box 
{
      public:
         static int objectCount;
         //定义构造方法
         Box(double l=2.0, double b=2.0, double h=2.0)
         {
                cout <<"Constructor called." << endl;
                length = l;
                breadth = b;
                height = h;
                // 每次创建对象时增加 1
                objectCount++;
         }
         double Volume()
         {
            return length * breadth * height;
         }
    private:
       double length;     // 长度
       double breadth;    // 宽度
       double height;     // 高度 
};
//初始化objectCount
int Box::objectCount = 0; //初始化

int main(void)
{
    Box Box1(3.3, 1.2, 1.5);    // 声明 box1
    Box Box2(8.5, 6.0, 2.0);    // 声明 box2

    //输出对象的参数
    cout << "Total objects: " << Box::objectCount << endl;
    return 0;
}