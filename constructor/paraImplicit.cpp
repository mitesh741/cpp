#include<iostream>
using namespace std;
class X
{
	private:
		int a,b;
	public:
		X(int m,int n)
		{
			a = m;    // parameterized constructor
			b = n;
		}
		void display()
		{
			cout << a << endl << b << endl;
		}
};
int main()
{
	X obj1(10,20);
	X obj2(30,40);
	X obj3(50,60);
	obj1.display();
	obj2.display();
	obj3.display();
	return 0;
}
