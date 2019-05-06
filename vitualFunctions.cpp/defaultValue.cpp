#include <iostream>
using namespace std;

class Base
{
	public:
		virtual void fun ( int x = 0 ) // Compiler substitutes default value at runtime. 
		{
			cout << "Base::fun(), x = " << x << endl;
		}
};

class Derived : public Base
{
	public:
		 void fun ( int x= 10 ) // IMP. Compiler always takes default value of base class virtual function's default value. So We should avoid taking default values in virtual functions.
		{
			cout << "Derived::fun(), x = " << x << endl;
		}
};

int main()
{
	Derived d1;
	Base *bp = &d1;
	bp->fun();
	return 0;
}
