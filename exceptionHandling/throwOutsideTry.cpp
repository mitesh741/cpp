#include<iostream>
using namespace std;
void test(int x)
{
	cout << "Inside test with value = " << x << endl;
	if(x)  throw x;
}
int main()
{
	try 
	{
		cout << "Inside try block " << endl;
		test(0);
		test(1);
		test(2);

	}
	catch(int i)
	{
		cout <<"Caught an exception values" <<i << endl; 
	}
	cout << "End program" << endl;
	return 0;
}
