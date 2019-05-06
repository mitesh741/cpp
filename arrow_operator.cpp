#include<iostream>
using namespace std;
class X
{
	private:
		int a;
		int b;
	public:
		void getData()
		{
			cout << "Enter the value for A= "; cin >> a;

			cout << "Enter the value for B= "; cin >> b;
		}
		void putData()
		{
			cout << "A = " << a << endl; // endl is an object 
			cout << "B = " << b << endl; // endl is an object 
		}
};
int main()
{
	X *p1, *p2;
	X obj1, obj2;

	p1 = &obj1;
	p2 = &obj2;
	p1->getData();
	p2->getData();

	p1->putData();
	p2->putData();
	return 0;
}


