#include<iostream>
using namespace std;
class X
{
	private:
		int a;
	protected:
		int b;
	public :
		int c;

		void abc_x()
		{
			a = 1;
			b = 2;
			c = 3;
		}
};
class Y : protected X
{
	public :
		void abc_y()
		{
//			a = 10; // Error Private so cant be accessed
			b =20; // protected remains protected 
			c = 30; // public becomes protected
			abc_x(); // public becomes protected
		}
};
class Z : public Y
{
	public :
		void abc_z()
		{
//			a =100; // private can not be accessed
			b =200; // inherited as protected 
			c =300;// inherited as protected
			abc_y(); // inherited as public
			abc_x(); // inherited as protected
		}
};
int main()
{
	Z ob1;

//	ob1.a =1000; // not inherited and hence can not be accessed
//	ob1.b =2000; // Protected data member can not be accessed
//	ob1.c =3000; // protected data member can not be acceseed
	ob1.abc_z(); 
	ob1.abc_y();
	return 0;
}
