#include <iostream>
using namespace std;
//C++����ʵ�ֶ�̳� ���� PHP ֻ��ʹ�õ��̳�
// ����
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

//����
class PaintCost {
	public:
		int getCost(int area){
			return area * 700;
		}
}; 

//������
class Rectangle :public Shape,public PaintCost{ //��̳�֮������ö��Ÿ��� 
	public:
		int getArea(){
			return (width * height);
		}
}; 

int main(void){
   
     	Rectangle Rect; //��������
		
		int area;
		
		Rect.setHeight(10);
		Rect.setWidth(10);
		
		area = Rect.getArea();
		//�����������
		cout << "Total area :" << Rect.getArea() <<endl;
		
		
		//����ܻ���
		cout << "total paint cost :$" << Rect.getCost(area) << endl; 
		return 0; 
     	
}

 
