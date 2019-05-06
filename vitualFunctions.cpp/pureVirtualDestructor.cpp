#include <iostream>
class Base
{
	public:
		  virtual ~Base() = 0 ; // Pure virtual destructor
};

Base::~Base()
{
	std::cout << "Pure virtual destructor is called\n";
}    // Generally Virtual function doesnt have definitions but since destructors are called in reverse order so without definition how destructor of base will be exectuted. so definition is required. 
class Derived : public Base
{
	public:
		~Derived()
		{
			std::cout << "~Derived() Constructor is executed\n";
		}
};

int main()
{
	Base *b=new Derived();
	delete b;
	return 0;
}
