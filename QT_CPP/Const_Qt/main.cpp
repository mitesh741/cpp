#include<iostream>
#include"position.h"

using namespace std;


void setX(Position &, int);
int main()
{


    Position dog(10,50);
    dog.getPosition();
    dog.setPosition(50,100);
    setX(dog,99);

    dog.getPosition();

    const Position house(100,200);
    house.getPosition();
    //        house.setPosition(444,100);
    //        house.getPosition();
    return 0;
}
void setX(Position &p,int i)
{
// Position p = dog ;
    p.x = i;
}
