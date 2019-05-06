#include <iostream>
#include "test.h"
using namespace std;

int main(int argc, char *argv[])
{
    B objB;
    A objA;

    objA.setSecretValue(objB, 70);
    cout << objB.getSecretValue() << endl;


    return 0;
}
