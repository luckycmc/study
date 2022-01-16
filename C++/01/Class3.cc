#include <iostream>
using namespace std;

class Box
{
	 public:
	 	double length;
	 	void setWith(double wid);
	 	double getWith(void);
	 	
    private:
       double with;
};
//成员函数
double Box::getWith(void)
{
	return with;
} 

void Box::setWith(double wid)
{
	with = wid;
}

int main()
{
	Box box;
	
	//不适用成员函数设置长度
	box.length = 10.0; //OK：因为length  是共有的
	cout << "Length of box :" << box.length <<endl;
	 
	   // box.width = 10.0; // Error: 因为 width 是私有的
    box.setWith(10.02);  // 使用成员函数设置宽度

	cout << "Width of box : " << box.getWith() <<endl;
    
}
