#include<iostream>

using namespace std;

class A
{
	public:
virtual		 void fun()
		{ 
			cout<<"\n A::fun() called ";
		}
};

class B: public A 
{
	public: 
		void fun() 
		{ 
			cout<<"\n B::fun() called "; 
		}      
};

class C: public B 
{
	public:
		void fun()
		{
			cout<<"\n C::fun() called "; 
		}      
};

int main()
{
	C c; // An object of class C
	B *b = &c; // A pointer of type B* pointing to c
	b->fun();  // this line prints "C::fun() called"
	return 0;
}

// Function once declared as virtual in Base class is always virtual in derived class also.
