#include<iostream>

using namespace std;

class base 
{
	public:
		base()     
		{ 
			cout<<"Constructing base \n";
		}
	virtual	~base() //  If you find any virtual function then you should add virtual destructor in base class.Try removing virtual.
		{ 
			cout<<"Destructing base \n"; 
		}     
};

class derived: public base 
{
	public:
		derived()     
		{
			cout<<"Constructing derived \n"; 
		}
		~derived()
		{
			cout<<"Destructing derived \n"; 
		}
};

int main(void)
{
	derived *d = new derived();  
	base *b = d;
	delete b;    // Here we are deleting object of base class. So destructor of derived class wont execute. To solve this declare base class destructor as Virtual. Try deleting with pointer b with and w/o virtual.
// Virtual helps deleting derived class just because pointer b has address of object derived. (This is same as you declared base class function virtual and you take pointer of base class and assign address of derived class and it will call function of that particular derived function that is overriden.)

	//derived d; // The constructor and destructor of both class will be called in correct order.

	return 0;
}
