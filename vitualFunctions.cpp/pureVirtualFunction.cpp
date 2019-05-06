#include<iostream>
using namespace std;
class Base
{
	public:
		virtual void show() = 0;
};



class Derived : public Base 
{
	public : void show()
		 {


			 cout << "Show func in derived called " << endl;
		 }
};

int main(void)
{
	Derived d;
	d.show();
	return 0;
}

// If we do not override pure virtual function in derived classes then derived class becomes abstract class.
