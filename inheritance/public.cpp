#include<iostream>
using namespace std;
class X
{
	private:
		int a;
	protected:
		int b;
	public:
		int c;
		void abc_x()
		{
			a = 1 ;
			b = 2;
			c = 3;
		}
		void show_x()
		{
			cout << a << endl;
			cout << b << endl;
			cout << c << endl;
		}
};
class Y : public X
{
	public:
		void abc_y()
		{
//			a =10; Error: a cant be accessed since its private
			b =20;
			c = 30;
			show_x();
		}
};
int main()
{
	Y ob2;

//	ob2.a =1000;   private  Error
//	ob2.b =2000;   Error: protected can only be accessed in derived class
	ob2.c =3000;

	ob2.abc_y();
	ob2.show_x();
	return 0;
}
