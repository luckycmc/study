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
//��Ա����
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
	
	//�����ó�Ա�������ó���
	box.length = 10.0; //OK����Ϊlength  �ǹ��е�
	cout << "Length of box :" << box.length <<endl;
	 
	   // box.width = 10.0; // Error: ��Ϊ width ��˽�е�
    box.setWith(10.02);  // ʹ�ó�Ա�������ÿ��

	cout << "Width of box : " << box.getWith() <<endl;
    
}
