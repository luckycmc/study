#include <iostream>
using namespace std;

class Box
{
	 public:
	 	 Box(double l=2.0, double b=2.0, double h=2.0)  //定义带参的构造方法 
		  {
		  	    cout <<"Constructor called." << endl;
		         length = l;
		         breadth = b;
		         height = h;
		  }
		  //计算体积 
		  double Volume()
		  {
		  	return  length *  breadth *  height; 
		  } 
		  
		  int compare(Box box)
		  {
		  	return this->Volume() > box.Volume();
		  }
		  //私有属性 
		private:
			  double length;     // Length of a box
		      double breadth;    // Breadth of a box
		      double height;     // Height of a box
};

int main()
{
	 Box Box1(3.3, 1.2, 1.5);    // Declare box1
     Box Box2(8.5, 6.0, 2.0);    // Declare box2
     //比较box1和box2 
     Box *ptrBox;
     
     return 0;
} 
