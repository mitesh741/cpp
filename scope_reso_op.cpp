#include<iostream>
using namespace std;
class interest
{
	private:
		double p,t,r,si;
	public:
		void getData();
		void compute();
		void putData();

};
void interest::getData()
{
	cout << "Enter p,t and r:" << endl;
	cin >> p >> t >> r;
}
void interest::compute()
{
	si = p * t * r /100;
}
void interest::putData()
{
	cout << "P =" << p << endl;
	cout << "Q =" << t << endl;
	cout << "R =" << r << endl;
	cout << "SI=" << si << endl;
}
int main()
{
	interest ob;

	ob.getData();
	ob.compute();
	ob.putData();
	return 0;
}
