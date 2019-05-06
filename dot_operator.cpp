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
	X obj1,obj2;
	obj1.getData();
	obj2.getData();

	obj1.putData();
	obj2.putData();
	return 0;
}
