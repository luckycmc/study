#include <iostream>

using namespace std;

namespace  A{
     int a =100;
     namespace  B{
        int a =20;
     }
}

int a = 200; //定义一个全局变量

int main(){
    cout << "A::a =" << A::a << endl;        //A::a =100
    cout << "A::B::a =" << A::B::a << endl;  //A::B::a =20
    cout << "a =" << a << endl;              //a =200
    cout << "::a =" << ::a << endl;          //::a =200

}