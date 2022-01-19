#include <iostream>
using namespace std;
/**********
 * 对象的指针引用类型
 * */
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
    Box *ptrBox;   //对象指针
     
	 //保存第一个对象的地址 
	ptrBox = &Box1;

		// 现在尝试使用成员访问运算符来访问成员 和struct 结构体指针 一样
	cout << "Volume of Box1: " << ptrBox->Volume() << endl;

	// 保存第二个对象的地址
	ptrBox = &Box2;

	// 现在尝试使用成员访问运算符来访问成员
	cout << "Volume of Box2: " << ptrBox->Volume() << endl;
	
	return 0;
} 
