#include<iostream>
using namespace std;
class X
{
	private:
		int a,b;
	public:
		X()
		{
			a = 10;
			b = 20;
		}
		void display()
		{
			cout << a << b << endl;
		}

};
int main()
{
	X obj1,obj2,obj3;
	obj1.display();
	obj2.display();
	obj3.display();
}
