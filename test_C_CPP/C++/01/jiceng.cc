#include<iostream>
using namespace std;
//����
class Shape{
	public:
		void setWith(int w){
			width = w;
		}
		void setHeight(int w){
			height = w;
		}
	protected:
		int width;
		int height;
}; 

//������̳л���   ��PHP��һ���ĵط��� PHP �̳��õ���extends  
// ��ͬ�㶼����ʹ�ø�������Ժͷ��� protcted ��public 
class Rectangle :public Shape{
	 
	  public:
	  	int getArea(){
	  		return width * height;
		}
}; 

int main(void)
{
	//ʵ������
	 class Rectangle Rect;  //Ҳ������class  Rectangle
	 Rect.setHeight(10); 
	 Rect.setWith(30);
	 
	 //�����������
	  cout << "total area :"<< Rect.getArea() << endl;
	  return 0;
	  
}
