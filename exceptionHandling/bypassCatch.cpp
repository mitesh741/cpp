#include<iostream>
using namespace std;
int main()
{


	try
	{
		cout << "Inside try block" << endl;
	}
	catch(int i)
	{
		cout << "Caught an exception with value = " << i << endl;
	}

	cout << "End " << endl;


	return 0 ;
}
