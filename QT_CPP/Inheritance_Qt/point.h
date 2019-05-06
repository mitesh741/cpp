#ifndef POINT_H
#define POINT_H


class Point
{
protected:
    int x;
public:

    Point(int = 0 );
    ~Point();

    int getX() {return x;}
    void setX(int);
};
class Point2D : public Point
{
protected:
    int y;

public:
    Point2D(int = 0, int  = 0);

    ~Point2D();
};
class Point3D : public Point2D
{
};
#endif // POINT_H
