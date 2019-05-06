#include<iostream>
using namespace std;
class X
{
	public:
		X()
		{
			cout << "Constructing base" << endl;
		}
		~X()
		{
			cout << "Destructing base " << endl;
		}
};
class Y: public X
{
	public:
		Y()
		{
			cout << "Constructing derived 1" << endl;
		}
		~Y()
		{
			cout << "Destructing derived 1 " << endl;
		}
};
class Z : public Y
{
	public:
		Z()
		{
			cout << "Constructing derived 2" << endl;
		}
		~Z()
		{
			cout << "Destructing derived 2" << endl;
		}
};
int main()
{
	Z ob;
}
