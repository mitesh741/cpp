#include<iostream>
using namespace std;
class two_numbers
{
	private:
		int a,b;
	public:
		two_numbers(int x,int y)
		{
			a = x;
			b = y;
		}
		friend class minimum;
};
class minimum
{
	public:
		int min(two_numbers p)
		{
			return(p.a < p.b ? p.a : p.b);
		}
};
int main()
{
	two_numbers ob1(20,30);
	minimum ob2;
	cout << "Minimum= " << ob2.min(ob1) <<endl;
	return 0;
}
