#include <iostream>

using namespace std;

class Line 
{
	public:
		double length;
		void   setLength(double len);
		double getLength(void);
};

//��Ա�����Ķ���
double Line::getLength(void)
{
	return length;
} 

//��Ա���� ���ó���
void  Line::setLength(double len)
{
	length = len;
} 

//����������
int main()
{
	Line line;
	//���ó���
	line.setLength(6.0);
	cout << "Line of line :" << line.getLength() <<endl; 
	
	//Ҳ����ֱ������
	line.length = 10.0;
	cout << "Line of line :" << line.getLength() <<endl; 

} 

