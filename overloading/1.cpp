#include<iostream>
using namespace std;
class Complex
{
	private:
		double i,r;
	public:
		void input();
		void display();
		Complex operator+(Complex ob2);
};
Complex Complex::operator+(Complex ob2)
{
	Complex temp;
	temp.r = r +ob2.r;
	temp.i = i +ob2.i;
	return temp;
}
void Complex::input()
{
	cout << "Enter real part: " << endl;
	cin >> r;

	cout << "Enter Imaginary part :" << endl;
	cin >> i;
}
void Complex::display()
{
	cout << r ;
	if(i > 0)
		cout << "+" << i << "i";
	else
		cout << i << "i";

	cout << endl;
}
int main()
{
	Complex x;
	x.input();
	Complex y;
	y.input();
	Complex z;
	z = x+y;

	cout << "X=" ;
	x.display();

	cout << "Y=" ;
	y.display();

	cout << "Z=" ;
	z.display();

	return 0 ;
}
