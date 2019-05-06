#include<iostream>
using namespace std;
int main()
{
	int a,b,c;
	try
	{
		cout << "Enter a and b" << endl;
		cin >> a >> b ;
		if(b ==0 ) throw 0;
		c = a/b ;
		cout << "Quotient = " << c << endl;
	}
	catch(int)
	{
		cout << "Divide by Zero" << endl;
	}
	return 0 ;
}
