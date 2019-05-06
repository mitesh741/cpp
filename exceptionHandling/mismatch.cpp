#include<iostream>
using namespace std;
int main()
{
	try
	{
		cout << "Inside try" << endl;
		throw 10;
		cout << "This will not execute" << endl;
	}
	catch(double x)
	{
		cout <<"Caught an exception with value = " << x << endl;
	}
	return 0;
}
