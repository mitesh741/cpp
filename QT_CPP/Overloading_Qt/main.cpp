#include <iostream>
#include "integer.h"
using namespace std;

int main(int argc, char *argv[])
{

    Integer a(60);

    int b = (int)a; //or int b = a;

    a = 100;  // Convert Constructor


    cout << a.getNr() << endl;
    cout << b << endl;


    a += b; // a = a + b;
    cout << a << endl;
    //    cout << a +(Integer) b << endl;
    return 0;
}
