#include <iostream>
#include "test.h"
using namespace std;

int main(int argc, char *argv[])
{
    Test *obj1 = Test::getInstance();
    //    Test *obj2= Test::getInstance();

    cout << "Address of obj1 " << obj1 << endl;

    Test obj2 = *obj1;  // Implicit Constructor . Here If there is no user defined copy constructor default constructor wont be called.


    *(obj1->p) = 700;  // Both objects have same address .so value at address is reflected in both objects if no copy constructor is called.

    cout << "Address of obj2 " << &obj2 << endl;

    cout << endl;
    cout << "obj1.x " << obj1->x << endl;
    cout << "*(obj1.p) " << *(obj1->p) << endl;
    cout << "obj1.p " << obj1->p << endl;
    cout << "obj1.y " << obj1->y << endl <<endl;

    cout << "obj2.x " << obj2.x << endl;
    cout << "*(obj2.p) " << *(obj2.p) << endl;
    cout << "obj2.p " << obj2.p << endl;
    cout << "obj2.y " << obj2.y << endl;

    return 0;
}
