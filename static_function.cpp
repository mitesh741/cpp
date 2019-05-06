#include<iostream>
using namespace std;
class X
{
	private:
		static int a;
		int c;
	public :
		static int b;
		static void display1()
		{
			cout << a <<endl;
			cout <<  b<< endl;
			cout << c;  // Static function can access only static data 
		}
		void display2()
		{
			c = 99;
			cout << a <<endl;
			cout <<  b <<endl;
			cout <<  c <<endl;
		}
};
int X::a=10;
int X::b=20;

int main()
{
	X ob;
	cout << X::b<< endl;

	X::display1();
	ob.display1();
	ob.display2();
	return 0;
}
