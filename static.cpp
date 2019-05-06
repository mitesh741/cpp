#include<iostream>
using namespace std;
class X
{
	private:
		static int a;
		int b;
	public:
		void initialize(int x,int y)
		{
			a = x;
			b = y;
		}
		void display()
		{
			cout << "Static a = " << a << endl;
			cout << "Non Static b = " << b << endl;
		}
};
int X::a;
int main()
{
	X ob1,ob2,ob3;

	ob1.initialize(10,20);
	ob1.display();
	ob2.initialize(30,40);
	ob2.display();
	ob3.initialize(50,60);
	ob3.display();

	cout << endl << "Final initialization of static a will be reflected to all the object" << endl;

	ob1.display(); // Here is the catch
	ob2.display();
	ob3.display();

	return 0;
}
