#ifndef POINT_H
#define POINT_H
#include <iostream>
using namespace std;

template<typename T>
class Point
{
protected:
    char b;
    T x;
public:
    Point(T = 0 );
    ~Point();
//    T getX() {return x;}
    T getX();
    void setX(T);
};
template<typename T>
Point<T>::Point(T x)
{
    this->x = x;
    cout << "The constructor from class Point has just been invoked" << endl;
}
template<typename K>
class Point<K*>
{
public:

    Point(K a = 0) {cout << "You cant use pointers here"<<  endl;}

};

template<typename T>
Point<T>::~Point()
{
    cout << "The destructor from class Point has just been invoked" << endl;

}
template<typename T>
void Point<T>::setX(T x)
{
    this->x = x;
}
template<typename T>
T Point<T>::getX()
{
    return this->x;
}
template<>
double Point<double>::getX() // Special Case
{
    cout << "This function will be executed only when Point<double> " << endl;
    return this->x ;
}
template<>
class Point<char>
{
  public:
    Point(char a = 0) {cout << "This is from Point<char>" ; }
};

template <typename T>
class Point2D : public Point<T>
{
protected :
    T y;
public:
    Point2D(T  = 0 , T = 0 );
    ~Point2D();
    T getY() {return y;}
    void setX(T);
    void setY(T);
    void setXY(T,T);
    Point2D operator+(Point2D);
};
template <typename T>
Point2D<T>::Point2D(T x,T y) : Point<T>(x) // : Point(x) helps initialize z with constructor of class Point
{
    this->y = y;
    cout << "The constructor from the point 2D class has just been invoked" << endl;
}
template <typename T>
Point2D<T>::~Point2D()
{
    cout << "The destructor from the point 2D class has just been invoked" << endl;
}
template <typename T>
void Point2D<T>::setX(T x)
{
    this->x = x;
    cout << "I am from Point2D" << endl;

}
template <typename T>
void Point2D<T>::setY(T y)
{
    this->y = y;
}
template <typename T>
void Point2D<T>::setXY(T x, T y)
{
    this->x = x;
    cout << "I am from Point2D" << endl;
}
template <typename T>
Point2D<T> Point2D<T>::operator+(Point2D o)
{
    Point2D temp;
    temp.setX(this->getX()+o.getX());
    temp.setY(this->getY()+o.getY());
    return temp;
}

#endif // POINT_H
