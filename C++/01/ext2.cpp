#include <iostream>
using namespace std;
//C++可以实现多继承 但是 PHP 只能使用单继承
// 基类
class Shape{
	public:
		void setWidth(int w){
			width = w;
		}
		void setHeight(int h){
		     height = h;	
		}
	protected:
		int width;
		int height;
		
}; 

//基类
class PaintCost {
	public:
		int getCost(int area){
			return area * 700;
		}
}; 

//派生类
class Rectangle :public Shape,public PaintCost{ //多继承之间的类用逗号隔开 
	public:
		int getArea(){
			return (width * height);
		}
}; 

int main(void){
   
     	Rectangle Rect; //创建对象
		
		int area;
		
		Rect.setHeight(10);
		Rect.setWidth(10);
		
		area = Rect.getArea();
		//输出对象的体积
		cout << "Total area :" << Rect.getArea() <<endl;
		
		
		//输出总花费
		cout << "total paint cost :$" << Rect.getCost(area) << endl; 
		return 0; 
     	
}

 
