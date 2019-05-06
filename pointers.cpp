#include<iostream>
using namespace std;
int main()
{
	int var = 1224;
	{
		int *p = new int;
		*p = 50;
		cout << *p << endl;
		delete p ;
		p = NULL;

		p = &var;

		if(p != NULL)
		{
			*p = 60;
			cout << *p << endl;
		}
		cout << "var : " << *p << endl;
	}
	cout << "var:" << var <<endl;
	return 0;
}
