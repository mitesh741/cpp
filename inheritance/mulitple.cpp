#include<iostream>
using namespace std;
class X
{
	private:
		int a;
	public:
		void inita()
		{
			a = 10;
		}
		void showa()
		{
			cout << a << endl;
		}
};
class Y
{
	private:
		int b;
	public:
		void initb()
		{
			b =20;
		}
		void showb()
		{
			cout << b << endl;
		}
};
class Z: public X, public Y
{
	private: 
		int c;
	public:
		void initc()
		{
			c = 30;
		}
		void showc()
		{
			cout << c << endl;
		}
};
int main()
{
	Z ob;
	ob.inita(); ob.showa();
	ob.initb(); ob.showb();
	ob.initc(); ob.showc();
	return 0;

}
