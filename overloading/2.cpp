#include <iostream>
using namespace std;

int add(int ,int);

double add(int ,int);

int main()
{
	cout << add(1,2) << endl; 
	cout << add(1.2,3.2) << endl;

	return 0 ;
}
int add(int a,int b)
{
	return a+b;
}
double add(int a,int b)
{
	return a+b;
}
