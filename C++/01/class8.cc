#include <iostream>
using namespace std;
class Box
{
	public:
		double  length;         //锟斤拷 
		double  breadth;        // 锟斤拷锟斤拷
		double   height;         // 锟竭讹拷
		//锟斤拷锟斤拷锟斤拷员锟斤拷锟斤拷
		double getVolue(void);
		void setLength( double len );
        void setBreadth( double bre );
        void setHeighth( double hei ); 
}; 

//锟斤拷员锟斤拷锟斤拷锟侥讹拷锟斤拷

double Box::getVolue(void)
{
	return length * breadth *height;
} 

void Box::setLength(double len)
{
	length = len;
}
void Box::setBreadth( double bre )
{
	breadth = bre;
}
void Box::setHeighth(double hei)
{
	height = hei;
}


int main()
{
	Box b1;
	double volume =0.0;
	b1.setBreadth(2.3);
	b1.setHeighth(2.0);
	b1.setLength(2.0); 
	volume = b1.getVolue();
	cout << "Box1 的体积是" << volume <<endl;
}

