#include "point.h"
#include <iostream>
using namespace std;
Point::Point(int x)
{
    this->x = x;
    cout << "The constructor from class has just been invoked" << endl;
}

void Point::setX(int x)
{
    this->x = x;
}
Point::~Point()
{
    cout << "The destructor from class has just been invoked" << endl;

}
Point2D::Point2D(int x,int y) : Point(x) // : Point(x) helps initialize z with constructor of class Point
{
    this->y = y;
    cout << "The constructor from the point 2D class has just been invoked" << endl;
}
Point2D::~Point2D()
{
    cout << "The destructor from the point 2D class has just been invoked" << endl;
}

void Point2D::setX(int x)
{
    this->x = x;
    cout << "I am from Point2D" << endl;

}

void Point2D::setY(int y)
{
    this->y = y;
}

void Point2D::setXY(int, int)
{
    this->x = x;
    cout << "I am from Point2D" << endl;
}

Point2D Point2D::operator+(Point2D o)
{
    Point2D temp;
    temp.setX(this->getX()+o.getX());
    temp.setY(this->getY()+o.getY());
    return temp;

}
