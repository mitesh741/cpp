class Base {};
class Derived : public Base {};
class UnrelatedClass {};

int main() 
{
	Base base;
	Derived derived;

	// #1: valid upcast
	Base *pBase = static_cast<Base *>(&derived);  

	// #2: valid downcast
	Derived *pDerived = static_cast<Derived *> (&base);

	// #3: invalid, between unrelated classes
//	UnrelatedClass *pUnrelated 
//		= static_cast<UnrelatedClass *> (&derived);
}
/*static_cast can be used to force implicit conversions such as non-const object to const, int to double. It can be also be used to perform the reverse of many conversions such as void* pointers to typed pointers, base pointers to derived pointers. But it cannot cast from const to non-const object. This can only be done by const_cast operator. */.
