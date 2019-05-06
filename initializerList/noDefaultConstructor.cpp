#include <iostream>
using namespace std;
class A 
{
	int i;
	public:
	A(int );
};
A::A(int arg) 
{
	i = arg;
	cout << "A's Constructor called: Value of i: " << i << endl;
}
class B 
{
	A a;      // class A doesn't have default constructor so initialization list must be used.
	public:
	B(int );
};

B::B(int x):a(x) 
{                        //Initializer list must be used
	cout << "B's Constructor called" << endl;
}

int main()
{
	B obj(10);
	return 0;
}
