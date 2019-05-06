#include<iostream>
using namespace std;
class X
{
	private:
		int i;
	public:
		int j,k;
		void seti(int p)
		{
			i = p;
		}
		void geti()
		{
			return i;
		}
};
class Y: private X
{
	public :
		X::j   // j is private but make it public by grantig access.
		X::seti();   // seti() is private but make it public
		X::geti();   // geti() is private but make it public

//		X::i // Error  We can not elevate private in base to public in derived
		int a; // public 
};
int main()
{
	Y ob;
//	ob.i = 10;
	ob.j =20;
//	ob.k = 30;
	ob.a = 40;
	ob.seti(10);

	cout << ob.geti() << " " <<   "  " << ob.a;
	return 0;
}
