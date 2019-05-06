#include <iostream>
#include "point.h"

using namespace std;

void operationOnPoints();

template<typename T,typename T2>
T2 add(T var1,T2 var2)
{
    return var1 + var2;
}
//template<>
//Point2D add(Point2D var1, Point2D var2)
//{
//    Point2D temp;
//    temp.setX(var1.getX()+var2.getX());
//    temp.setY(var1.getY()+var2.getY());
//    return temp;
//}
//int add(int var1,int var2)
//{

//    return (var1 + var2);
//}
int main(int argc, char *argv[])
{
    operationOnPoints();


    //    cout << add<double,double>(2,5.4) << endl;
//    cout << static_cast<int>(6.7) << endl;
    return 0;
}

void operationOnPoints()
{
    Point2D p1(10,67);

    Point2D p2(50,3);

    Point2D sum =  add(p1,p2);

    cout << sum.getX() << endl;
    cout << sum.getY() << endl;

}
