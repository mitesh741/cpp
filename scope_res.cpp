#include<iostream>
using namespace std;
void printData()
{
	cout << "Array elements are" << endl;
}
class X
{
	private:
		int a[6];
	public:
		int n;
		void getData();
		void printData(int n);
};
void X::printData(int n)
{
	if(n == -1)
	{
		::printData();
		return;
	}
	printData(n-1);
	cout << a[n] << endl;
}
void X::getData()
{
	cout << "Enter the value for n=";
	cin >> n;
	cout << "Enter " << n << "Elements" << endl;
	for(int i =0 ; i < n ; i++)
	{
		cin >> a[i];
	}
}
int main()
{
	X ob;
	ob.getData();
	ob.printData(ob.n-1);
	return 0;
}
