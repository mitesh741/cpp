#include<iostream>
using namespace std;
class X
{
	int a;
	public :
	X(int p = 0 )
	{
		a = p;
	}
	void seta(int p = 0 )
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
	public :
	Y(int q = 0 , int r = 0 ): X(q)
	{
		b = r;
	}
	void setb(int q = 0 )
	{
		b =q;
	}
	int getb()
	{
		return b;
	}
};
int main()
{
	Y ob(20,100);
// Compiler consideration
// Y ob; 
// ob.X(20) Constructor of base class X is invoked 
// ob.Y(100) Constructor of base class X is invoked 

	cout << ob.geta() << endl;
	cout << ob.getb() << endl;
	return 0;
}

