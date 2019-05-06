#include<iostream>
using namespace std;
int i =20;
int main()
{
	int i =10;
	cout << "Local value of i= " << i << endl;
	cout << "Global value of i= " << ::i << endl;
	return 0;
}
