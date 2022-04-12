#include <iostream>
using namespace std;
class Box
{
	public:
		double  length;         //�� 
		double  breadth;        // ����
		double   height;         // �߶�
		//������Ա����
		double getVolue(void);
		void setLength( double len );
        void setBreadth( double bre );
        void setHeighth( double hei ); 
}; 

//��Ա�����Ķ���

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
	cout << "Box1 �������" << volume <<endl;
}

