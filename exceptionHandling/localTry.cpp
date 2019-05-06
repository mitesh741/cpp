#include<iostream>
using namespace std;
void test(int x)
{
	try
	{
		if(x) throw x;
	}
	catch(int i)
	{
		cout << " Caught execption number:" << i << endl;
	}
}
int main()
{
	cout << "Start of program " << endl;
	test(1);
	test(2);
	test(0);
	test(3);

	cout << "End" << endl;
	return 0;

}
