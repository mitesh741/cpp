#include<iostream>
using namespace std;
class X
{
	private:
		int a,b;
	public:
		void set_ab(int i,int j);
		friend int add(X obj);
};
void X::set_ab(int i,int j)
{
	a = i;
	b = j;
}
int add(X obj)
{
	return obj.a + obj.b;
}
int main()
{
	X ob1;
	ob1.set_ab(10,20);
	cout << add(ob1) << endl;
	return 0;
}
