#include "test.h"
#include <iostream>
using namespace std;
Test* Test::instance=0;

Test::Test(int x, int y, int value)
{
    this->x = x;
    this->y = y;

    this->p = new int(value);  //Address is copied and not the value.  Both objects will point to same address

    cout << "this->p " << p << endl ;
}
Test::~Test()
{
    delete p;
}
Test::Test(const Test & obj)
{
    this->p =new int(*( obj.p));

    //    cout << "obj.p  " << obj.p << endl;

    cout << "new int(*(obj.p))" << this->p << endl;
    this->x = obj.x;
    this->y = obj.y;
}
Test* Test::getInstance()
{
    if(!instance)
    {
        instance = new Test(2,3,4);
        cout << "Address of instance in get Instace" << instance << endl;
        cout << "getInstace: First\n" ;
        return instance;
    }
    else
    {
        cout << "getInstance: Second\n";
        return instance;
    }
}

