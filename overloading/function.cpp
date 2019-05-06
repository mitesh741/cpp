#include<iostream>
using namespace std;
class A
{
	protected :
		int x;
		int y;
	public :
		A() 
		{}
		void read()
		{
			cout << "Enter values of x in class A:";
			cin >> x;
			cout << "Enter values of y in class A:";
			cin >> y;
		}
		void show()
		{
			cout << "In class A, x = " << x << endl;
			cout << "In class A, y = " << y << endl;
		}
};
class B: public A
{
	protected :
		int y;
		int z;
	public:
		void read()
		{
			A::read();
			cout << "Enter value of y in class B" ;
			cin >> y;
			cout << "Enter value of z in class B" ;
			cin >> z;
		}
		void show()
		{
			A::show();
			cout << "In class B, y = " << y << endl;
			cout << "In class B, z = " << z << endl;
			cout << "y of class A shown rom B" << A::y << endl;
		}
};
int main()
{
	B obj;
	cout << "Input the data for the derived object" << endl;
	obj.read();
	cout << "Output the contents of  the derived object" << endl;
	obj.show();
	return 0;
}
