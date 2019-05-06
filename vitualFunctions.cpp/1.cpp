#include<iostream>
using namespace std;
class X
{
	public : 
		void show()
		{
			cout << " This is base class X " << endl;
		}
};
class Y : public X
{
	public :
		void show()
		{
			cout << " This is derived class Y " << endl;
		}
};
class Z : public Y
{
	public : 
		void show()
		{
			cout << " This is derived class Z " << endl;
		}
};
int main()
{
	X *p;
	X obj1;
	p = &obj1;
	p->show();

	Y obj2;
	p = &obj2;
	p->show();

	Z obj3;
	p = &obj3;
	p->show();
	return 0;
}// In this programs compiler will decide which function has to invoke by types of pointer
// and not by the type of address of the object stored in pointer


