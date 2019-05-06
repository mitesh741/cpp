#include <iostream>
#include "point.h"

using namespace std;

void operationOnPoints();

int main(int argc, char *argv[])
{
     operationOnPoints();

    cout << "Hello World!" << endl;
    return 0;
}

void operationOnPoints()
{
    Point2D p1;
    cout << p1.getX() << endl;
}
