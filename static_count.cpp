#include<iostream>
using namespace std;
class X
{
	public:
		static int count;
		X()
		{
			count++;
		}
};
int X::count;
int main()
{
	X obj1;
	cout << "count " << obj1.count << endl;  // At any point of time static data member contains number of objects existing

	X obj2;
	cout << "count " << obj2.count << endl;

	X obj3;
	cout << "count " << obj3.count << endl;
	return 0;
}

