#include<position.h>
#include<iostream>
using namespace std;
Position::Position(int x,int y)
{
	this->x = x;
	this->y = y;
}
Position::~Position()
{

}
void Position::getPosition()
{
	cout << "x  : "  << x << endl;
	cout << "Y  : "  << y << endl;
}
void Position::setPosition(int x,int y)
{
	this->x = x;
	this->y = y;

}
