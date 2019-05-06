#include<iostream>
using namespace std;
int main()
{
	int x = 10;
	int &y = x;
	cout << x << endl;
	cout << y << endl;

	y= 20;
	cout << y << endl;
	cout << x << endl;

	x =30;
	cout << y << endl;
	cout << x << endl;


	return 0;
}





