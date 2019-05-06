
#include<iostream>
using namespace std;
class base
{
        public :
                int a;
};
class derived1:virtual  public base
{
        public:
                int b;
};
class derived2: virtual public base
{
        public:
                int c;
};
class derived3: public derived1, public derived2
{
        public:
                int d;
};
int main()
{
        base baseobj;
        derived1 d1obj;
        derived2 d2obj;
        derived3 d3obj;

        d3obj.a = 10; // due to virtual it is allowed
        d3obj.b = 20;
        d3obj.c = 30;

        d3obj.d = d3obj.a + d3obj.b + d3obj.c; //same here

cout << "Size of base  " << sizeof(baseobj) << endl;
cout << "Size of derived1  " << sizeof(d1obj) << endl;
cout << "Size of derived2  " << sizeof(d2obj) << endl;
cout << "Size of d3  " << sizeof(d3obj) << endl;


        cout << baseobj.a << endl;
        cout <<"d1obj.a  "  << d1obj.a << endl;
        cout << "d1obj.b  " << d1obj.b << endl;
        cout << "d2obj.a  " << d2obj.a << endl;
        cout << "d2obj.c  " << d2obj.c << endl;
        cout << "d3obj.a  " << d3obj.a << endl ;
        cout << "d3obj.b  " << d3obj.b << endl;
        cout << "d3obj.c  " << d3obj.c << endl;
        cout << "d3obj.d  " << d3obj.d << endl;
        return 0;
}
    
