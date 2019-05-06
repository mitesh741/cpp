#include<iostream>
using namespace std;
class A
{
	int m;
	public:
	void setm(int i)
	{
		m = i;
	}
	void displaym()
	{
		cout << m << endl;
	}
};
class B: public A
{
	int n;
	public : 
	void setn(int j)
	{
		n = j;
	}
	void displayn()
	{
		cout << n << endl;
	}
};
int main()
{
	A x;
	x.setm(100);
	x.displaym();

	B y;
	y.setm(10);
	y.displaym();
	y.setn(20);
	y.displayn();

	cout << "Size of Base class = " << sizeof(A) << endl;
	cout << "Size of Derived class = " << sizeof(B) << endl;

	return 0;
}
