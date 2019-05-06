#include<iostream>
using namespace std;
void test(int n)
{
	try
	{
		switch(n)
		{
			case 1:
				throw 1;
			case 2:
				throw 2.1243;
			case 3:
				throw "Hello";
		}
	}
	catch(int i)
	{
		cout << "Caught integer exception:  " << i << endl;
	}
	catch(double d)
	{
		cout << "Caught double exception:  " << d << endl;
	}
	catch(char *str)
	{
		cout << "Caught String exception:  " << str << endl;
	}
}
int main()
{
	int choice ;
	for(;;)
	{
		cout << "1 : integer  2:float   3:String  4:Exit" << endl;
		cout << "Enter choice" ; 
		cin >> choice;

		switch(choice)
		{
			case 1:
			case 2:
			case 3:
				test(choice);
				break;
			default:
				cout << "Program is terminated" << endl;
		}
	}
	return 0 ;
}
