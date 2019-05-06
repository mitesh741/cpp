#include<iostream>
using namespace std;
class X
{
	private:
		int a,b;
	public:
		void initialize()
		{
			a = 10;
			b = 20;
		}
		void display()
		{
		cout << a <<endl<< b << endl;
		}
};
int main()
{
	X obj1,obj2,obj3;
	obj1.initialize();
	obj2.initialize();
	obj3.initialize();

	obj1.display();
	obj2.display();
	obj3.display();
	return 0;
}
/* THe initialization member func should be called explicitly usinfg an object. This is very
inconvenient if there are large number of objects.  */
