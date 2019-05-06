#include "point.h"
#include <iostream>

using namespace std;

Point::Point(int x)
{
    this->x = x;
    cout << "The constructor from Point class has just been invoked" << endl;
}
Point::~Point()
{
    cout << "The destructor from Point class has just been invoked" << endl;

}
void Point::setX(int x)
{
    this->x = x;
}

Point2D::Point2D(int x ,int y) : Point(x)
{
cout << "The constructor from derived class has just been invoked"<< endl;
    this->x = x;
    this->y = y;

}
Point2D::~Point2D()
{
    cout << "The destructor from derived class has just been invoked"<< endl;

}
