//The dynamic_cast operator, which safely converts from a pointer (or reference) to a base type to a pointer (or reference) to a derived type.
//
//
//dynamic_cast is used to perform safe downcasting, i.e., to determine whether an object is of a particular type in an inheritance hierarchy. It is the only cast that may have a significant runtime cost.

#include <iostream>
using namespace std;

class A
{
	public:
		virtual void f(){cout << "A::f()" << endl;}
};

class B : public A
{
	public:
		void f(){cout << "B::f()" << endl;}
};

int main()
{
	A a;
	B b;
	a.f();        // A::f()
	b.f();        // B::f()

	A *pA = &a;   
	B *pB = &b;   
	pA->f();      // A::f()
	pB->f();      // B::f()

	pA = &b;
	// pB = &a;      // not allowed
	pB = dynamic_cast<B*>(&a); // allowed but it returns NULL

	return 0;
}

/*The downcasting, the opposite of upcasting, is a process converting a base-class pointer or reference to a derived-class pointer or reference.

It is not allowed without an explicit type cast. That's because a derived class could add new data members, and the class member functions that used these data members wouldn't apply to the base class. */
