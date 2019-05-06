#include<iostream>
using namespace std;
class X
{
	protected:
		int a;
	public:
		void seta()
		{
			a =10;
		}
		void showa()
		{
			cout << "a = " << a << endl;
		}
};
int main()
{
	X ob;
//	ob.a = 20;  // Can not access protected member;
	ob.seta();
	ob.showa();
	return 0 ;
}
