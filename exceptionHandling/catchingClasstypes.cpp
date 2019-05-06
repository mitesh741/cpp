#include<iostream>
#include<string>
#include<cstring>

using namespace std;
class X
{
	public:
		char str[80];
		int n;
		X()
		{
			*str = 0 ;
			n = 0 ;
		}
		X(char *s, int m)
		{
			std::strcpy(str,s);
			n = m;
		}
};
int main()
{
	int i ;
	try
	{
		cout << "Enter  +ve integer " << endl;
		cin >> i;

		if(i < 0 ) throw X("Not positive", i);

	}
	catch(X a)
	{
		cout << "Caught exception :" << a.str << endl;
		cout << "with value :" << a.n << endl;

	}
	return 0 ;
}
