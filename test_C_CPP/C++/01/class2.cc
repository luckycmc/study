#include <iostream>

using namespace std;

class Line 
{
	public:
		double length;
		void   setLength(double len);
		double getLength(void);
};
/*************实现Line类的方法 start*************/
//成员函数的定义
double Line::getLength(void)
{
	return length;
} 

//成员函数 设置长度
void  Line::setLength(double len)
{
	length = len;
} 
/*************实现Line类的方法 end*************/
//程序主函数
int main()
{
	Line line;
	//设置长度
	line.setLength(6.0);
	cout << "Line of line :" << line.getLength() <<endl; 
	
	//也可以直接设置
	line.length = 10.0;
	cout << "Line of line :" << line.getLength() <<endl; 

} 

