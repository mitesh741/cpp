#include<iostream>
using namespace std;
class X
{
	private:
		int a,b;
	public:
		X(int m,int n)
		{
			a = m;
			b = n;
		}
		void display()
		{
			cout << a << endl << b << endl;
		}
};
int main()
{
	X obj1 = X(10,20);
	X obj2 = X(30,40);
	X obj3 = X(50,60);

	obj1.display();
	obj2.display();
	obj3.display();
	return 0;
}

