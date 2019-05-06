#include<iostream>
using namespace std;
class X
{
	int a;
	public:
	X(int p = 23)
	{
		a = p;
	}
	void seta(int  p = 0)
	{
		a = p;
	}
	int geta()
	{
		return a;
	}
};
class Y : public X
{
	int b;
	public:
	Y(int q = 0)
	{
		b = q;
	}
	void setb(int  q = 0)
	{
		b = q;
	}
	int getb()
	{
		return b;
	}
};
int main()
{
	Y ob(100); // Allocates memory for Y
	// next call the constructor of base class w/o any parameter
	// ob.X();
	// Call to the constructor of derived class will initiailze
	// ob.Y(100);
	cout << " ob.geta()" << ob.geta() << endl;
	cout << " ob.getb()" << ob.getb() << endl;
	return 0;
}
