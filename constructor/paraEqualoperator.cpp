#include<iostream>
using namespace std;
class X
{
	private:
		int a;
	public:
		X(int m)
		{
			a = m;
		}
		void display()
		{
			cout << a << endl;
		}
};
int main()
{
	X obj1 = 10;
	X obj2 = 20;
	X obj3 = 30;
	obj1.display();
	obj2.display();
	obj3.display();
	return 0;
}
