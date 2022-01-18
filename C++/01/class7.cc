#include <iostream>
using namespace std;

//基类
class Shape
{
     public:
      void setWidth(int w)
      {
         width = w;
      }
      void setHeight(int h)
      {
         height = h;
      }
   protected:
      int width;
      int height;
};

//派生类
class Reactangle :public Shape
{
     public:
        int getArea()
        {
            return (width * height);
        }
};

int main()
{
    Reactangle Rect; //实例化一个对象
    Rect.setWidth(5);
    Rect.setHeight(7);

    //输出对象的体积
    cout << "Reactangle area " << Rect.getArea() << endl;
    return 0;
}