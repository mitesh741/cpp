#include<iostream>
using namespace std;
class X
{
	public:
		static int a;
};
int X::a;
int main()
{
	X::a = 20;
	cout << "Initial value of a= " << X::a << endl;

	X b;
	cout << "value of a using object b= " << b.a << endl;

	return 0;
}
// The static data member exist much before any object of its class is created. This is because they 
// can be treated as global variables which can be accessed by non member function
