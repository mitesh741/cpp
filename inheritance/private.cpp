#include<iostream>
using namespace std;
class X
{
	private :
		int a;
	protected:
		int b;
	public:
		int c;
		void abc_x()
		{
			a=1;
			b=2;
			c=3;
		}
};
class Y: private X
{
	public:
		void abc_y()
		{
//			a =10; Error: Private and hence cant be accessed 
			b =20;
			c =30;
			abc_x();
		}
};
class Z:public Y
{
	public: 
		void abc_z()
		{
		//	a =100; // Error private and hence cant not be accessed
		//	b =200;
		//	c =300;
			abc_y();
//			abc_x();
		}
};
int main()
{
	Z ob;
//	ob.a = 100;
//	ob.a = 200;
//	ob.a = 300;

	ob.abc_z();
//	ob.abc_x();
	ob.abc_y();

	return 0;

}
